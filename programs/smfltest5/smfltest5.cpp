#include <iostream>
#include <boost/program_options.hpp>
#include "SagoDataHolder.hpp"
#include "SagoSprite.hpp"
#include <physfs.h>         //Abstract file system. To use containers
#include "MoveInput.hpp"
#include "GameCharacter.hpp"
#include "GameDraw.hpp"
#include "FrameCounter.hpp"


using namespace std;

void ReadMovementKeys(MoveInput &input) {
	input.action = false;
	input.down = false;
	input.left = false;
	input.right = false;
	input.up = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		input.down = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		input.left = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		input.right = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		input.up = true;
	}
}

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
	SagoDataHolder texHolder;	
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test 5");
	FrameCounter fc(texHolder);
	GameDraw gd;
	gd.render = &window;
	gd.texHolder = &texHolder;
	sf::Clock clock;  //start the clock
	sf::Int32 lastFrameTime = 0;
	MoveInput input;
	GameCharacter player;
	player.race = "male";
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
		ReadMovementKeys(input);
		player.Update(input,fDeltaTime);
		window.clear();
		gd.Draw(player,frameTime);
		fc.Draw(window,frameTime);
		window.display();
	}
	PHYSFS_deinit();
	return 0;
}
