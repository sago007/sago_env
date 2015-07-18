#include <iostream>
#include <boost/program_options.hpp>
#include <cppdb/frontend.h>

using namespace std;

static void printGames(cppdb::session& session)
{
	cppdb::result res = session << "select gamenumber from oastat_games where gamenumber not in (select gamenumber from oastat_points WHERE score > 0)";
	while (res.next()) {
		res >> cout;
		cout << endl;
	}
}

int main(int argc, const char* const argv[])
{
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print basic usage information to stdout and quits")
		("dbargs", boost::program_options::value<string>(), "Arguments passed to the DB backend")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}
	string connectstring = "mysql:database=oastat";
	if (vm.count("dbargs")) {
		connectstring = vm["dbargs"].as<string>();
	}
	cppdb::session sql(connectstring);
	printGames(sql);
	return 0;
}
