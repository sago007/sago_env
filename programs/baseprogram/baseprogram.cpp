#include <iostream>
#include <boost/program_options.hpp>

using namespace std;

int main(int argc, const char* argv[]) {
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
	("help,h", "Print basic usage information to stdout and quits")
	("somestring", boost::program_options::value<string>(), "A string to print")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}
	if (vm.count("somestring")) {
		string somestring = vm["somestring"].as<string>();
		cout << "Called with a parameter value of: " << somestring << endl;
	}
	else {
		cout << "This program demonstrates boost::program_options. Try \"" << argv[0] << " --help\" or \"" << argv[0] << " --somestring hello\"" << endl;
	}
	return 0;
}
