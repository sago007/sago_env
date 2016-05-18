#include <iostream>
#include <boost/program_options.hpp>
#include "sago/dbsync.hpp"
#include <cereal/archives/json.hpp>

using std::string;
using std::cout;
using std::vector;

int main(int argc, const char* argv[])
{
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print basic usage information to stdout and quits")
		("somestring", boost::program_options::value<string>(), "A string to print")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		cout << desc << "\n";
		return 1;
	}
	if (vm.count("somestring")) {
		string somestring = vm["somestring"].as<string>();
		cout << "Called with a parameter value of: " << somestring << "\n";
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
	
	return 0;
}
