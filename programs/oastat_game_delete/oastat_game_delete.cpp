#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include <cppdb/frontend.h>

using namespace std;

static void DeleteGame(cppdb::session& session, int g) 
{
	session << "DELETE FROM oastat_accuracy WHERE gamenumber = ?" << g << cppdb::exec;
	session << "DELETE FROM oastat_challenges WHERE gamenumber = ?" << g << cppdb::exec;
	session << "DELETE FROM oastat_gamecvars WHERE gamenumber = ?" << g << cppdb::exec;
	session << "DELETE FROM oastat_kills WHERE gamenumber = ?" << g << cppdb::exec;
	session << "DELETE FROM oastat_points WHERE gamenumber = ?" << g << cppdb::exec;
	session << "DELETE FROM oastat_team_events WHERE gamenumber = ?" << g << cppdb::exec;
	session << "DELETE FROM oastat_awards WHERE gamenumber = ?" << g << cppdb::exec;
	session << "DELETE FROM oastat_games WHERE gamenumber = ?" << g << cppdb::exec; 
}

static void DeleteGames(cppdb::session& session, const vector<int>& games) 
{
	for (int g : games) {
		cppdb::transaction t(session);
		DeleteGame(session, g);
		t.commit();
	}
}

int main(int argc, const char* const argv[])
{
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print basic usage information to stdout and quits")
		("dbargs", boost::program_options::value<string>(), "Arguments passed to the DB backend")
		("game", boost::program_options::value<vector<int> >(), "Game to delete. By default arguments are considered a game number given to this command")
	;
	boost::program_options::positional_options_description p;
	p.add("game", -1);
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
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
	if (vm.count("game")) {
		vector<int> games = vm["game"].as<vector<int> >();
		DeleteGames(sql, games);
		return 0;
	}
	cerr << "Missing paramter. See --help for details" << endl;
	return 1;
}
