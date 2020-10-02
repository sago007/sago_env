#include <iostream>
#include <boost/program_options.hpp>
#include <ncurses.h>

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

int main(int argc, const char* argv[]) {
	boost::program_options::options_description desc("Options");
	desc.add_options()
	("version", "Print version information and quit")
	("help,h", "Print basic usage information to stdout and quit")
	("somestring", boost::program_options::value<std::string>(), "A string to print")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "ncurses_hello_world " << VERSIONNUMBER << "\n";
		return 0;
	}
	initscr();
	std::string msg = "Hello World !!!";
	if (vm.count("somestring")) {
		msg = vm["somestring"].as<std::string>().c_str();
	}
	bool shutdown = false;
	int lastKey = 0;
	while(!shutdown) {
		wclear(stdscr);
		printw("%s", msg.c_str());
		wmove(stdscr, 2, 0);
		printw("Last key pressed: %d", lastKey);
		wmove(stdscr, 5, 10);
		printw("Press Esc to quit!");
		refresh();
		int key = wgetch(stdscr);
		lastKey = key;
		if (key == 27) {
			shutdown = true;
		}
	}
	endwin();
	return 0;
}
