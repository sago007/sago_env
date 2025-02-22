#include <iostream>
#include <boost/program_options.hpp>
#include "LogicalResize.hpp"

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

void test_logical_resize(const LogicalResize& logical_resize, int x, int y) {
	int result_x;
	int result_y;
	logical_resize.LogicalToPhysical(x, y, result_x, result_y);
	std::cout << "x: " << x << " y: " << y << " result_x: " << result_x << " result_y: " << result_y << " top margin:" << logical_resize.GetTopMargin() << std::endl;
}

int main(int argc, const char* argv[]) {
	boost::program_options::options_description desc("Options");
	desc.add_options()
	("version", "Print version information and quit")
	("help,h", "Print basic usage information to stdout and quit")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "logical_resize " << VERSIONNUMBER << "\n";
		return 0;
	}
	LogicalResize logical_resize(1200, 600);
	logical_resize.SetPhysicalSize(1200, 1200);
	test_logical_resize(logical_resize, 300, 150);
	test_logical_resize(logical_resize, 600, 300);

	logical_resize.SetPhysicalSize(1800, 600);
	test_logical_resize(logical_resize, 300, 150);
	test_logical_resize(logical_resize, 600, 300);

	logical_resize.SetPhysicalSize(3600, 1200);
	test_logical_resize(logical_resize, 300, 150);
	test_logical_resize(logical_resize, 600, 300);

	logical_resize.SetPhysicalSize(1800, 900);
	test_logical_resize(logical_resize, 13, 150);

	const int tile_size = 13;
	for (int i = 0; i < 10; i++) {
		int p1 = i * tile_size;
		int p2 = tile_size;
		//logical_resize.LogicalToPhysical(&p1, nullptr, &p2, nullptr);
		p2 += p1;
		std::cout << "p1: " << p1 << " p2: " << p2 << std::endl;

	}

	
	return 0;
}
