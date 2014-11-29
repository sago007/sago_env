#include <iostream>
#include <boost/program_options.hpp>
#include "sago/GameStateManager.hpp"
#include "sago/SagoDataHolder.hpp"
#include "sago/SagoMenu.hpp"
#include "sago/SagoCommandQueue.hpp"
#include "sago/SagoSprite.hpp"
#include "sago/SagoSpriteHolder.hpp"
#include "sago/FrameCounter.hpp"
#include "sago/SagoMusicBackground.hpp"
#include <SFML/Graphics.hpp>
#include <physfs.h>

using namespace std;

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
		cout << desc << endl;
		return 1;
	}
	PHYSFS_init(argv[0]);
	std::string baseSearchPath = PHYSFS_getBaseDir();
	baseSearchPath += "/data/";
	PHYSFS_addToSearchPath(baseSearchPath.c_str(),0);
	sago::SagoDataHolder dataHolder;
	sago::SagoSpriteHolder spriteHolder(dataHolder);
	sago::SagoCommandQueue cmdQ;
	sago::music::SetDataHolder(dataHolder);
	sago::FrameCounter fc(dataHolder);
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Sago test 1");
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
		cmdQ.ReadKeysAndAddCommands(window);
		cmdQ.ClearCommands();
		for (size_t i = 0; i < cmdQ.GetCommandQueue().size();i++) {
			std::string cmd = cmdQ.GetCommandQueue().at(i);
			if (cmd == "QUIT") {
				window.close();
			}
		}
		window.clear();
		fc.Draw(window,frameTime);
		window.display();
		usleep(1000);
	}
	PHYSFS_deinit();
	return 0;
}
