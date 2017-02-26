#include <iostream>
#include <boost/program_options.hpp>
#include <SDL2/SDL_mixer.h>
#include "sago/SagoDataHolder.hpp"
#include "sago/SagoSpriteHolder.hpp"

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

void runGame() {
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* bitmapTex = NULL;
	int posX = 100, posY = 100, width = 640, height = 480;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_OGG);
}

int main(int argc, const char* argv[]) {
	PHYSFS_init(argv[0]);
	PHYSFS_addToSearchPath(PHYSFS_getBaseDir(),1);
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
		std::cout << "tiledtest " << VERSIONNUMBER << "\n";
		return 0;
	}
	runGame();
	return 0;
}
