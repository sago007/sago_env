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
			cout << value;
			for (int i = 1; i< res.cols(); ++i) {
				value.clear();
				res >> value;
				cout << ", " << value;
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
	("printargs", "Prints all arguments and terminates. This includes parameters featched from environment variables and default values")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
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
	const char* connectstring_env = getenv("SAGO_CONNECTION_STRING");
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
	if (vm.count("printargs")) {
		cout << "connectstring: " << cmdargs.connectstring << "\n";
		cout << "sqlstring: " << cmdargs.sqlstring << "\n";
		cout << "minCount: " << cmdargs.minCount << "\n";
		cout << "maxCount: " << cmdargs.maxCount << "\n";
 		return 0;
	}
	return DoStuff();
}
