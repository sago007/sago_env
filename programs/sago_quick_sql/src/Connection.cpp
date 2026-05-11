#include "Connection.hpp"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <sago/platform_folders.h>

const std::string SAGO_PREFIX = "sago:";

std::string ExtractAlias(const std::string& connectstring) {
	if (connectstring.substr(0, SAGO_PREFIX.size()) != SAGO_PREFIX) {
		return std::string();
	}
	return connectstring.substr(SAGO_PREFIX.size());
}

void ResolveConnectionAlias(std::string& connectstring) {
	if (connectstring.substr(0, SAGO_PREFIX.size()) != SAGO_PREFIX) {
		return;
	}
	std::string alias = connectstring.substr(SAGO_PREFIX.size());
	if (alias.empty()) {
		throw std::runtime_error("Empty alias in connection string");
	}
	std::string path = sago::getConfigHome() + "/sago_quick_sql/connections/" + alias + ".txt";
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open connection file: " + path);
	}
	std::string line;
	if (!std::getline(file, line) || line.empty()) {
		throw std::runtime_error("Connection file is empty: " + path);
	}
	connectstring = line;
}

void LogRequest(const std::string& alias, const CommandArguments& args) {
	std::string logDir = sago::getCacheDir() + "/sago_quick_sql/logs";
	std::filesystem::create_directories(logDir);
	std::string logFile = logDir + "/" + (alias.empty() ? "generic" : alias) + ".log";
	std::ofstream out(logFile, std::ios::app);
	if (!out.is_open()) {
		return;
	}
	char timebuf[64];
	std::time_t now = std::time(nullptr);
	std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%dT%H:%M:%S", std::localtime(&now));
	out << timebuf << " TYPE=" << (args.doExec ? "EXEC" : "SELECT") << " SQL=\"\"\"" << args.sqlstring << "\"\"\"";
	for (const auto& b : args.bindArgs) {
		out << " BIND=\"" << b << "\"";
	}
	out << "\n";
}
