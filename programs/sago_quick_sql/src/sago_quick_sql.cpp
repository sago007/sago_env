#include <iostream>
#include <boost/program_options.hpp>
#include "CommandArguments.hpp"
#include <cppdb/frontend.h>

using std::string;
using std::vector;
using std::cout;

CommandArguments cmdargs;

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

//Environment to ensure that it is spelled consistently
const char* const SAGO_CONNECTION_STRING = "SAGO_CONNECTION_STRING";

static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from' (like " replaced with ""))
    }
    return str;
}

static void WriteCsvValue(std::ostream& out, std::string value) {
	bool mustAddQuotes = false;
	if (value.find("\"") != value.npos) {
		mustAddQuotes = true;
	}
	if (value.find(",") != value.npos) {
		mustAddQuotes = true;
	}
	if (value.find("\n") != value.npos) {
		mustAddQuotes = true;
	}
	if (mustAddQuotes) {
		//In CSV quotes are escaped with another quote
		value = ReplaceAll(value, "\"", "\"\"");
	}
	if (mustAddQuotes) {
		out << "\"" << value << "\"";
	}
	else {
		out << value;
	}
}

static void StripControlChars(std::string& value, bool allowLineShifts = false ) {
	for (size_t i = 0; i < value.size(); ++i) {
		char theChar = value.at(i);
		if (theChar < ' ') {
			if (theChar == '\n' && allowLineShifts) {
				continue;
			}
			value.erase(i, 1);
			--i;  //Go a position back because the string has shifted left by 1
		}
	}
}

static void PrintValue(const std::string& value) {
	if (cmdargs.csvOutput) {
		WriteCsvValue(std::cout, value);
	}
	else {
		std::cout << value;
	}
}

int DoSelect() {
	cppdb::session database(cmdargs.connectstring);
	database.begin();
	cppdb::statement st = database.prepare(cmdargs.sqlstring);
	for (const auto& b : cmdargs.bindArgs) {
		st.bind(b);
	}
	cppdb::result res = st.query();
	std::string value;
	int count = 0;
	while (res.next()) {
		value.clear();
		res >> value;
		if (!cmdargs.silent) {
			if (cmdargs.stripControlChars) {
				StripControlChars(value);
			}
			PrintValue(value);
			for (int i = 1; i< res.cols(); ++i) {
				value.clear();
				res >> value;
				if (cmdargs.stripControlChars) {
					StripControlChars(value);
				}
				cout << ",";
				PrintValue(value);
			}
			cout << "\n";
		}
		count++;
	}
	database.rollback();
	if (count < cmdargs.minCount) {
		return 1;
	}
	return 0;
}

int DoExec() {
	cppdb::session database(cmdargs.connectstring);
	database.begin();
	cppdb::statement st = database.prepare(cmdargs.sqlstring);
	for (const auto& b : cmdargs.bindArgs) {
		st.bind(b);
	}
	st.exec();
	auto numberOfRowsAffected = st.affected();
	cout << numberOfRowsAffected << "\n";
	database.commit();
	return 0;
}

int DoStuff() {
	if (cmdargs.doExec) {
		return DoExec();
	}
	return DoSelect();
}

int main(int argc, const char* argv[]) {
	boost::program_options::options_description desc("Options");
	desc.add_options()
	("version", "Print version information and quit")
	("help,h", "Print basic usage information to stdout and quit")
	("silent,s", "Do not write output")
	("connect-string", boost::program_options::value<std::string>(), "String used for connecting. Like \"odbc:DSN=mydb\"")
	("sql", boost::program_options::value<std::string>(), "SQL to use for select or execute")
	("bind", boost::program_options::value<std::vector<std::string>>(), "Binds to the sql")
	("exec", "Do an exec of a UPDATE, INSERT, DELETE or DDL instead of a SELECT")
	("min", boost::program_options::value<int>(), "Return non-zero if a select returns fewer results")
	("max", boost::program_options::value<int>(), "Return non-zero if a select returns more results (ignored if negative)")
	("csv", "Write the output in CSV format. This will escape the quotes and quote items with commas")
	("strip-control", "Removes all control charectors including tab and lf (line feed / new line)")
	("printargs", "Prints all arguments and terminates. This includes parameters featched from environment variables and default values")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		std::cout << "\n";
		std::cout << "If --connect-string is not given, the system will try the environment variable " << SAGO_CONNECTION_STRING << " instead.\n";
		std::cout << "If that is not given either. The default value of \"" << cmdargs.connectstring << "\" will be used\n";
		if (getenv(SAGO_CONNECTION_STRING)) {
			std::cout << "The environment " << SAGO_CONNECTION_STRING << " is currently set\n";
		}
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "sago_quick_sql " << VERSIONNUMBER << "\n";
		return 0;
	}
	if (vm.count("silent")) {
		cmdargs.silent = true;
	}
	bool connectstring_set = false;
	const char* connectstring_env = getenv(SAGO_CONNECTION_STRING);
	if (connectstring_env) {
		cmdargs.connectstring = connectstring_env;
		connectstring_set = true;
	}
	if (vm.count("connect-string")) {
		cmdargs.connectstring = vm["connect-string"].as<string>();
		connectstring_set = true;
	}
	if (!connectstring_set) {
		std::cerr << "connect-string not set, using default: \"" << cmdargs.connectstring << "\"\n";
	}
	if (vm.count("sql")) {
		cmdargs.sqlstring = vm["sql"].as<string>();
	}
	if (vm.count("bind")) {
		cmdargs.bindArgs = vm["bind"].as< vector<string> >();
	}
	if (vm.count("exec")) {
		cmdargs.doExec = true;
	}
	if (vm.count("min")) {
		cmdargs.minCount = vm["min"].as<int>();
	}
	if (vm.count("max")) {
		cmdargs.maxCount = vm["max"].as<int>();
	}
	if (vm.count("csv")) {
		cmdargs.csvOutput = true;
	}
	if (vm.count("strip-control")) {
		cmdargs.stripControlChars = true;
	}
	if (vm.count("printargs")) {
		cout << "connectstring: " << cmdargs.connectstring << "\n";
		cout << "sqlstring: " << cmdargs.sqlstring << "\n";
		cout << "minCount: " << cmdargs.minCount << "\n";
		cout << "maxCount: " << cmdargs.maxCount << "\n";
 		return 0;
	}
	if (cmdargs.sqlstring.empty()) {
		std::cerr << "An \"--sql\" argument must be given\n";
		return 1;
	}
	return DoStuff();
}
