#include <iostream>
#include <boost/program_options.hpp>
#include "TextureHolder.hpp"
#include "SagoSprite.hpp"
#include <SFML/Graphics.hpp>

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
	TextureHolder texHolder;
	SagoSprite spUp(texHolder,"golem-walk",sf::IntRect(0,64*0,64,64),7,100);
	SagoSprite spLeft(texHolder,"golem-walk",sf::IntRect(0,64*1,64,64),7,100);
	SagoSprite spDown(texHolder,"golem-walk",sf::IntRect(0,64*2,64,64),7,100);
	SagoSprite spRight(texHolder,"golem-walk",sf::IntRect(0,64*3,64,64),7,100);
	SagoSprite spRightFast(texHolder,"golem-walk",sf::IntRect(0,64*3,64,64),7,50); //A varaint that walks twise as fast
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test 3");
	sf::Clock clock;  //start the clock
	while (window.isOpen()) {
		sf::Int32 frameTime = clock.getElapsedTime().asMilliseconds();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();
		spUp.Draw(window,frameTime,100,100);
		spLeft.Draw(window,frameTime,100,200);
		spRight.Draw(window,frameTime,100,300);
		spRightFast.Draw(window,frameTime,200,300);
		spDown.Draw(window,frameTime,100,400);
		window.display();
	}
	return 0;
}
