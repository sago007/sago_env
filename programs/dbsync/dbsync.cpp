#include <iostream>
#include <boost/program_options.hpp>
#include "sago/dbsync.hpp"
#include <cereal/archives/json.hpp>
#include "sago/DbSyncDbOracle.hpp"

using std::string;
using std::cout;
using std::vector;

int main(int argc, const char* argv[])
{
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print basic usage information to stdout and quits")
		("connectstring", boost::program_options::value<string>(), "A string for use with ")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		cout << desc << "\n";
		return 1;
	}
	string connectstring;
	if (vm.count("connectstring")) {
		connectstring = vm["connectstring"].as<string>();
	}
	
	sago::database::DbTable t;
	t.tablename = "MyTable";
	sago::database::DbColumn c;
	c.name = "name";
	c.type = sago::database::DbType::TEXT;
	c.length = 50;
	t.columns.push_back(c);
	{
		cereal::JSONOutputArchive archive( cout );
		archive ( cereal::make_nvp("total",t));
	}
	
	std::shared_ptr<cppdb::session> db(new cppdb::session(connectstring));
	DbSyncDbOracle oracle(db);
	cout << "SOME_TABLE: " << oracle.TableExists("SOME TABLE") << "\n";
	cout << "SYSTEM_JOB_PARAMETERS: " << oracle.TableExists("SYSTEM_JOB_PARAMETERS") << "\n";
	
	return 0;
}
