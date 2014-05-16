#include <iostream>
#include <boost/program_options.hpp>
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
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	float circlex = 0.0f;
	float circley = 0.0f;
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			circlex -= 1.0f*(fDeltaTime/4.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			circlex += 1.0f*(fDeltaTime/4.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			circley -= 1.0f*(fDeltaTime/4.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			circley += 1.0f*(fDeltaTime/4.0f);
		}
		shape.setPosition(circlex,circley);
		window.clear();
		window.draw(shape);
		window.display();
	}
	return 0;
}
