#include <iostream>
#include <SDL2/SDL_mixer.h>
#include "sago/SagoDataHolder.hpp"
#include "sago/SagoSpriteHolder.hpp"
#include "Libs/NFont.h"

using namespace std;

int main(int argc, const char* argv[]) {
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* bitmapTex = NULL;
	int posX = 100, posY = 100, width = 640, height = 480;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_OGG);
	PHYSFS_init(argv[0]);
	PHYSFS_permitSymbolicLinks(1);  //Would not be used in real life but saves space here
	PHYSFS_addToSearchPath(PHYSFS_getBaseDir(),1);

	win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	sago::SagoDataHolder holder(renderer);
	sago::SagoSpriteHolder spriteHolder(holder);

	const sago::SagoSprite& redSprite = spriteHolder.GetSprite("block_red");
	TTF_Font* ttf_font = holder.getFontPtr("freeserif", 16);
	if (!ttf_font) {
		cerr << "Font is null" << endl;
	}
	NFont nffont(renderer, ttf_font);
	//NFont nffont(renderer, "fonts/freeserif.ttf", 16);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
		cerr << "Mix_OpenAudio: " << Mix_GetError() << endl;
	}
	if (Mix_PlayMusic(holder.getMusicPtr("highbeat"), -1)==-1) {
		cerr << "Mix_OpenAudio: " << Mix_GetError() << endl;
		// well, there's no music, but most games don't break without music...
	}

	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
			if (e.type == SDL_KEYDOWN ) {
				Mix_PlayChannel( 1, holder.getSoundPtr("counter"), 0);
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
		for (int i=0; i< 5; ++i) {
			redSprite.Draw(renderer, 1, i*50, i*50);
		}
		SDL_Rect r = {50,0,50,50};
		SDL_Rect r2 = {0,50,50,50};
		redSprite.DrawBounded(renderer, 1, 25, 25, r);
		redSprite.DrawBounded(renderer, 1, 25, 25, r2);
		nffont.draw(renderer, 10, 10, NFont::Color(255,255,255,255), "This is a text");
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
