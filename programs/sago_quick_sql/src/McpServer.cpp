#include "McpServer.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cppdb/frontend.h>
#include <json/json.h>
#include <sago/platform_folders.h>

#include "CommandArguments.hpp"
#include "Connection.hpp"

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

namespace {

const char* const MCP_PROTOCOL_VERSION = "2024-11-05";

Json::Value MakeError(int code, const std::string& message) {
	Json::Value err(Json::objectValue);
	err["code"] = code;
	err["message"] = message;
	return err;
}

Json::Value MakeResponse(const Json::Value& id, const Json::Value& result) {
	Json::Value resp(Json::objectValue);
	resp["jsonrpc"] = "2.0";
	resp["id"] = id;
	resp["result"] = result;
	return resp;
}

Json::Value MakeErrorResponse(const Json::Value& id, int code, const std::string& message) {
	Json::Value resp(Json::objectValue);
	resp["jsonrpc"] = "2.0";
	resp["id"] = id;
	resp["error"] = MakeError(code, message);
	return resp;
}

// Wraps a string into the MCP tool result content shape and writes it
// into the given result object. If isError is true, marks the result as
// a tool execution error per MCP convention.
Json::Value MakeToolResult(const std::string& text, bool isError = false) {
	Json::Value content(Json::objectValue);
	content["type"] = "text";
	content["text"] = text;

	Json::Value contentArr(Json::arrayValue);
	contentArr.append(content);

	Json::Value result(Json::objectValue);
	result["content"] = contentArr;
	if (isError) {
		result["isError"] = true;
	}
	return result;
}

std::string SerializeCompact(const Json::Value& value) {
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "";
	builder["commentStyle"] = "None";
	return Json::writeString(builder, value);
}

void WriteMessage(const Json::Value& message) {
	std::cout << SerializeCompact(message) << "\n";
	std::cout.flush();
}

// Extracts the bind array from the params; tolerates missing or null,
// rejects non-array values, requires every element to be a string.
std::vector<std::string> ExtractBinds(const Json::Value& arguments) {
	std::vector<std::string> binds;
	if (!arguments.isMember("binds") || arguments["binds"].isNull()) {
		return binds;
	}
	const Json::Value& arr = arguments["binds"];
	if (!arr.isArray()) {
		throw std::runtime_error("'binds' must be an array of strings");
	}
	binds.reserve(arr.size());
	for (const auto& v : arr) {
		if (!v.isString()) {
			throw std::runtime_error("'binds' entries must be strings");
		}
		binds.push_back(v.asString());
	}
	return binds;
}

std::string RequireString(const Json::Value& arguments, const char* key) {
	if (!arguments.isMember(key) || !arguments[key].isString()) {
		throw std::runtime_error(std::string("Missing required string argument: ") + key);
	}
	return arguments[key].asString();
}

// Builds a tool input schema with alias + sql + optional binds.
Json::Value BuildSqlToolSchema() {
	Json::Value schema(Json::objectValue);
	schema["type"] = "object";

	Json::Value props(Json::objectValue);

	Json::Value aliasProp(Json::objectValue);
	aliasProp["type"] = "string";
	aliasProp["description"] = "Configured sago alias name (without the 'sago:' prefix). The connection string is read from <config_home>/sago_quick_sql/connections/<alias>.txt.";
	props["alias"] = aliasProp;

	Json::Value sqlProp(Json::objectValue);
	sqlProp["type"] = "string";
	sqlProp["description"] = "SQL statement, may contain '?' placeholders for positional binds.";
	props["sql"] = sqlProp;

	Json::Value bindsProp(Json::objectValue);
	bindsProp["type"] = "array";
	Json::Value bindItems(Json::objectValue);
	bindItems["type"] = "string";
	bindsProp["items"] = bindItems;
	bindsProp["description"] = "Positional bind parameters as strings.";
	props["binds"] = bindsProp;

	schema["properties"] = props;

	Json::Value required(Json::arrayValue);
	required.append("alias");
	required.append("sql");
	schema["required"] = required;

	return schema;
}

Json::Value BuildEmptySchema() {
	Json::Value schema(Json::objectValue);
	schema["type"] = "object";
	schema["properties"] = Json::Value(Json::objectValue);
	return schema;
}

Json::Value BuildToolsList() {
	Json::Value tools(Json::arrayValue);

	{
		Json::Value t(Json::objectValue);
		t["name"] = "sql_select";
		t["description"] = "Run a SELECT (or any read-only query) against a configured sago alias. The transaction is rolled back. Returns rows as a JSON array of objects keyed by column name.";
		t["inputSchema"] = BuildSqlToolSchema();
		tools.append(t);
	}
	{
		Json::Value t(Json::objectValue);
		t["name"] = "sql_exec";
		t["description"] = "Run an INSERT/UPDATE/DELETE/DDL statement against a configured sago alias. The transaction is committed on success. Returns the number of affected rows.";
		t["inputSchema"] = BuildSqlToolSchema();
		tools.append(t);
	}
	{
		Json::Value t(Json::objectValue);
		t["name"] = "list_aliases";
		t["description"] = "List configured sago aliases (file stems in <config_home>/sago_quick_sql/connections/*.txt).";
		t["inputSchema"] = BuildEmptySchema();
		tools.append(t);
	}

	Json::Value result(Json::objectValue);
	result["tools"] = tools;
	return result;
}

Json::Value ToolSqlSelect(const Json::Value& arguments) {
	std::string alias = RequireString(arguments, "alias");
	std::string sql = RequireString(arguments, "sql");
	std::vector<std::string> binds = ExtractBinds(arguments);

	CommandArguments logArgs;
	logArgs.connectstring = SAGO_PREFIX + alias;
	logArgs.sqlstring = sql;
	logArgs.bindArgs = binds;
	logArgs.doExec = false;
	LogRequest(alias, logArgs);

	std::string connectstring = SAGO_PREFIX + alias;
	ResolveConnectionAlias(connectstring);

	cppdb::session database(connectstring);
	database.begin();
	cppdb::statement st = database.prepare(sql);
	for (const auto& b : binds) {
		st.bind(b);
	}
	cppdb::result res = st.query();

	Json::Value rows(Json::arrayValue);
	const int cols = res.cols();
	std::vector<std::string> colNames;
	colNames.reserve(cols);
	for (int i = 0; i < cols; ++i) {
		colNames.push_back(res.name(i));
	}
	while (res.next()) {
		Json::Value row(Json::objectValue);
		for (int i = 0; i < cols; ++i) {
			if (res.is_null(i)) {
				row[colNames[i]] = Json::Value::null;
			}
			else {
				std::string value;
				res.fetch(i, value);
				row[colNames[i]] = value;
			}
		}
		rows.append(row);
	}
	database.rollback();

	return MakeToolResult(SerializeCompact(rows));
}

Json::Value ToolSqlExec(const Json::Value& arguments) {
	std::string alias = RequireString(arguments, "alias");
	std::string sql = RequireString(arguments, "sql");
	std::vector<std::string> binds = ExtractBinds(arguments);

	CommandArguments logArgs;
	logArgs.connectstring = SAGO_PREFIX + alias;
	logArgs.sqlstring = sql;
	logArgs.bindArgs = binds;
	logArgs.doExec = true;
	LogRequest(alias, logArgs);

	std::string connectstring = SAGO_PREFIX + alias;
	ResolveConnectionAlias(connectstring);

	cppdb::session database(connectstring);
	database.begin();
	cppdb::statement st = database.prepare(sql);
	for (const auto& b : binds) {
		st.bind(b);
	}
	st.exec();
	auto affected = st.affected();
	database.commit();

	Json::Value out(Json::objectValue);
	out["affected"] = static_cast<Json::Int64>(affected);
	return MakeToolResult(SerializeCompact(out));
}

Json::Value ToolListAliases(const Json::Value& /*arguments*/) {
	std::string dir = sago::getConfigHome() + "/sago_quick_sql/connections";
	Json::Value aliases(Json::arrayValue);
	std::error_code ec;
	if (std::filesystem::exists(dir, ec) && std::filesystem::is_directory(dir, ec)) {
		for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
			if (!entry.is_regular_file()) {
				continue;
			}
			const auto& path = entry.path();
			if (path.extension() == ".txt") {
				aliases.append(path.stem().string());
			}
		}
	}
	Json::Value out(Json::objectValue);
	out["aliases"] = aliases;
	out["directory"] = dir;
	return MakeToolResult(SerializeCompact(out));
}

Json::Value HandleToolsCall(const Json::Value& params) {
	if (!params.isObject() || !params.isMember("name") || !params["name"].isString()) {
		throw std::runtime_error("tools/call requires a string 'name'");
	}
	std::string name = params["name"].asString();
	Json::Value arguments = params.isMember("arguments") ? params["arguments"] : Json::Value(Json::objectValue);
	if (!arguments.isObject()) {
		throw std::runtime_error("tools/call 'arguments' must be an object");
	}

	if (name == "sql_select") {
		return ToolSqlSelect(arguments);
	}
	if (name == "sql_exec") {
		return ToolSqlExec(arguments);
	}
	if (name == "list_aliases") {
		return ToolListAliases(arguments);
	}
	throw std::runtime_error("Unknown tool: " + name);
}

Json::Value HandleInitialize(const Json::Value& /*params*/) {
	Json::Value result(Json::objectValue);
	result["protocolVersion"] = MCP_PROTOCOL_VERSION;

	Json::Value capabilities(Json::objectValue);
	capabilities["tools"] = Json::Value(Json::objectValue);
	result["capabilities"] = capabilities;

	Json::Value serverInfo(Json::objectValue);
	serverInfo["name"] = "sago_quick_sql";
	serverInfo["version"] = VERSIONNUMBER;
	result["serverInfo"] = serverInfo;

	return result;
}

bool IsNotification(const Json::Value& message) {
	return !message.isMember("id") || message["id"].isNull();
}

void HandleMessage(const Json::Value& message) {
	Json::Value id = message.isMember("id") ? message["id"] : Json::Value(Json::nullValue);
	const bool notification = IsNotification(message);

	if (!message.isMember("method") || !message["method"].isString()) {
		if (!notification) {
			WriteMessage(MakeErrorResponse(id, -32600, "Invalid Request: missing method"));
		}
		return;
	}

	std::string method = message["method"].asString();
	Json::Value params = message.isMember("params") ? message["params"] : Json::Value(Json::objectValue);

	// Notifications get no response.
	if (notification) {
		// notifications/initialized and similar: nothing to do.
		return;
	}

	try {
		if (method == "initialize") {
			WriteMessage(MakeResponse(id, HandleInitialize(params)));
			return;
		}
		if (method == "tools/list") {
			WriteMessage(MakeResponse(id, BuildToolsList()));
			return;
		}
		if (method == "tools/call") {
			// Tool execution failures are reported as tool results with
			// isError=true rather than JSON-RPC errors, per MCP convention.
			try {
				WriteMessage(MakeResponse(id, HandleToolsCall(params)));
			}
			catch (const std::exception& ex) {
				WriteMessage(MakeResponse(id, MakeToolResult(ex.what(), /*isError=*/true)));
			}
			return;
		}
		if (method == "ping") {
			WriteMessage(MakeResponse(id, Json::Value(Json::objectValue)));
			return;
		}
		WriteMessage(MakeErrorResponse(id, -32601, "Method not found: " + method));
	}
	catch (const std::exception& ex) {
		WriteMessage(MakeErrorResponse(id, -32603, std::string("Internal error: ") + ex.what()));
	}
}

}  // namespace

int RunMcpServer() {
	std::cerr << "sago_quick_sql MCP server started (stdio, protocol " << MCP_PROTOCOL_VERSION << ")\n";
	std::string line;
	Json::CharReaderBuilder readerBuilder;
	while (std::getline(std::cin, line)) {
		if (line.empty()) {
			continue;
		}
		Json::Value message;
		std::string parseErrors;
		std::istringstream stream(line);
		if (!Json::parseFromStream(readerBuilder, stream, &message, &parseErrors)) {
			WriteMessage(MakeErrorResponse(Json::Value(Json::nullValue), -32700, "Parse error: " + parseErrors));
			continue;
		}
		HandleMessage(message);
	}
	return 0;
}
