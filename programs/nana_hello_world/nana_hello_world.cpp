#include <iostream>
#include <boost/program_options.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>

int runNana()
{
	//Define a form object, class form will create a window
	//when a form instance is created.
	//The new window default visibility is false.
	nana::form fm;
	//Define a label on the fm(form) with a specified area,
	//and set the caption.
	nana::label lb{ fm, nana::rectangle{ 10, 10, 100, 100 } };
	lb.caption("Hello, world!");
	//Expose the form.
	fm.show();
	//Pass the control of the application to Nana's event
	//service. It blocks the execution for dispatching user
	//input until the form is closed.
	nana::exec();
	return 0;
}

int main(int argc, const char* const argv[]) {
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
	("help,h", "Print basic usage information to stdout and quits")
	("version,V", "Print version information")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "nana_hello_world 1.0.0\n";
		return 0;
	}
	return runNana();
}
