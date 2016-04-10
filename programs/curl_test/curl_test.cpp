#include <iostream>
#include <boost/program_options.hpp>
#include "sago/CurlSago.hpp"

int main(int argc, const char* const argv[]) {
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
	("help,h", "Print basic usage information to stdout and quits")
	("version,V", "Print version information")
	("url", boost::program_options::value<std::string>(), "Url to get")
	("add-header", boost::program_options::value<std::vector<std::string>>(), "Append header")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "curl_test 0.1.0" << std::endl;
		return 0;
	}
	std::string url = "http://files.poulsander.com/~poul19/";
	if (vm.count("url")) {
		url = vm["url"].as<std::string>();
	}
	else {
		std::cout << "Missing url" << std::endl;
	}
	sago::CurlSago cs;
	if (vm.count("add-header")) {
		auto headers = vm["add-header"].as<std::vector<std::string>>();
		for (const std::string& h : headers) {
			cs.headersIn.append(h.c_str());
		}
	}
	cs.SetUrl(url);
	std::cout << cs.PerformGet() << std::endl;
	std::cout << cs.http_code << std::endl;
	
	return 0;
}
