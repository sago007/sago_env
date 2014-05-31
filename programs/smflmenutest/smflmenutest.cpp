#include <iostream>
#include <boost/program_options.hpp>
#include <SFML/Graphics.hpp>
#include "SagoDataHolder.hpp"
#include "SagoMenu.hpp"
#include "SagoCommandQueue.hpp"
#include <physfs.h>         //Abstract file system. To use containers

using namespace std;

int main(int argc, const char* argv[])
{
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print basic usage information to stdout and quits")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}
	//Init the file system abstraction layer
	PHYSFS_init(argv[0]);
	PHYSFS_addToSearchPath("./",0);
	sago::SagoDataHolder texHolder;	
	sf::Text menutext;
	menutext.setFont(*texHolder.getFontPtr("FreeSerif"));
	menutext.setCharacterSize(24);
	menutext.setColor(sf::Color::White);
	sago::menu::SagoMenuStack menustack;
	sago::menu::SagoMenu menu(menutext,false);
	sago::menu::SagoMenuButton b;
	b.SetLabel("Single player - endless");
	b.SetCommand("SP_ENDLESS");
	menu.addButton(b);
	b.SetLabel("Single player - time trial");
	b.SetCommand("SP_TT");
	menu.addButton(b);
	menustack.PushMenu(menu);
	sago::menu::SagoMenu yes(menutext,true);
	b.SetLabel("Yes");
	b.SetCommand("YES_YES");
	yes.addButton(b);
	sago::SagoCommandQueue cmdQ;
	cmdQ.BindKey(sf::Keyboard::Up,"UP");
	cmdQ.BindKeyCommand("UP","UP");
	cmdQ.BindKey(sf::Keyboard::Down,"DOWN");
	cmdQ.BindKeyCommand("DOWN","DOWN");
	cmdQ.BindKey(sf::Keyboard::Return,"RETURN");
	cmdQ.BindKeyCommand("RETURN","CONFIRM");
	cmdQ.BindKey(sf::Keyboard::Escape,"ESC");
	cmdQ.BindKeyCommand("ESC","QUIT");
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Menu Test");
	sf::Clock clock;  //start the clock
	sf::Int32 lastFrameTime = 0;
	while (window.isOpen()) {
		sf::Int32 frameTime = clock.getElapsedTime().asMilliseconds();
		sf::Int32 deltaTime = frameTime - lastFrameTime;
		float fDeltaTime = static_cast<float>(deltaTime);
		lastFrameTime = frameTime;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		cmdQ.ReadKeysAndAddCommands();
		for (size_t i = 0; i < cmdQ.GetCommandQueue().size();i++) {
			std::string cmd = cmdQ.GetCommandQueue().at(i);
			if (cmd == "POP_MENU") {
				menustack.PopMenu();
			}
			if (cmd == "SP_TT") {
				menustack.PushMenu(yes);
			}
			if (cmd == "UP") {
				menustack.Up();
			}
			if (cmd == "DOWN") {
				menustack.Down();
			}
			if (cmd == "CONFIRM") {
				menustack.Action(cmdQ);
			}
			if (cmd == "QUIT") {
				window.close();
			}
			std::cout << cmd << std::endl;
		}
		cmdQ.ClearCommands();
		window.clear();
		menustack.DrawMenu(window);
		window.display();
	}
	
	PHYSFS_deinit();
	return 0;
}
