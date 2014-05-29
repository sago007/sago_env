#include <iostream>
#include <boost/program_options.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <unistd.h>

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
sf::Clock clock;  //start the clock
	sf::Int32 lastFrameTime = 0;
	sf::Font myfont;
	string fontname = "FreeSerif.ttf";
	if (!myfont.loadFromFile(fontname))
	{
		cerr << "Failed to load " << fontname << endl;
		return 2;
	}
	sf::Texture golemTex;
	if (!golemTex.loadFromFile("golem-walk.png")) {
		cerr << "Failed to load " << "golem-walk.png" << endl;
	}
	sf::Sprite spriteLeft;
	spriteLeft.setTexture(golemTex);
	int direction = 1; //0=up, 1=left,2=down,3=right 
	spriteLeft.setTextureRect(sf::IntRect(0,64*direction,64,64));
	sf::Text fpstext;
	fpstext.setFont(myfont);
	fpstext.setString("FPS");
	fpstext.setCharacterSize(24);
	fpstext.setColor(sf::Color::Red);
	sf::Int32 lastFPSupdate = 0;
	int fpscount = 0;
	float circlex = 0.0f;
	float circley = 0.0f;
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
		bool moving = false;
		//Speed is too fast then moving diagonally. This is an excercise for a later test
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			circlex -= 1.0f*(fDeltaTime/4.0f);
			direction = 1;
			moving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			circlex += 1.0f*(fDeltaTime/4.0f);
			direction = 3;
			moving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			circley -= 1.0f*(fDeltaTime/4.0f);
			direction = 0;
			moving = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			circley += 1.0f*(fDeltaTime/4.0f);
			direction = 2;
			moving = true;
		}
		if (moving) {
			spriteLeft.setTextureRect(sf::IntRect(0+64*((frameTime/100)%7),64*direction,64,64));
		}
		else {
			spriteLeft.setTextureRect(sf::IntRect(0,64*direction,64,64));
		}
		spriteLeft.setPosition(circlex,circley);
		fpscount++;
		if ( frameTime > lastFPSupdate + 1000 ) {
			stringstream ss;
			ss << "FPS: " << fpscount;
			fpstext.setString(ss.str());
			lastFPSupdate = frameTime;
			fpscount = 0;
		}
		window.clear();
		window.draw(spriteLeft);
		window.draw(fpstext);
		window.display();
		usleep(10000);
	}
	return 0;
}
