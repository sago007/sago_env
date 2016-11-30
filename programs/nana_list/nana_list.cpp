#include <iostream>
#include <boost/program_options.hpp>
#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <functional>

using std::cout;

static int selection = -1;

static void listSelected(const nana::arg_listbox& arg, nana::listbox* lbox)
{
	if (arg.item.selected()) {
		selection = arg.item.pos().item;
		std::cout << "Selected " << selection << "\n";
		for (int i = 0; i < static_cast<int>(lbox->at(0).size()); ++i) {
			auto item = lbox->at(0).at(i);
			if (i != selection) {
				cout << "Deselecting " << i << "\n";
				item.select(false);
			}
		}
	}
}

int runNana()
{
	//Define a form object, class form will create a window
	//when a form instance is created.
	//The new window default visibility is false.
	nana::form fm;
	nana::listbox l{fm, nana::rectangle{10, 40, 200, 200}};
	
	l.append_header("Key");
    l.append_header("Value");
	
	auto cat = l.at(0);
	cat.push_back("First key");
	cat.at(0).text(1, "First Value");
	cat.push_back("second key");
	cat.at(1).text(1, "Second Value");
	
	nana::place layout(fm);
	layout.div("vert<<here>><b height=10>");
	layout["here"] << l;
    layout.collocate();
	
	//Expose the form.
	fm.show();
	l.events().category_dbl_click([](const nana::arg_listbox_category& arg) {
		//Double click does not appear to work.
		std::cout << arg.category.position() << "!\n";
	});
	auto f1 = std::bind(listSelected, std::placeholders::_1, &l);
	l.events().selected( f1 );
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
		std::cout << "nana_list 0.0.1\n";
		return 0;
	}
	return runNana();
}
