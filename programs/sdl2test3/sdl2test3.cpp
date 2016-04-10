#include <iostream>
#include "sago/SagoDataHolder.hpp"
#include "sago/SagoSpriteHolder.hpp"

using namespace std;

int main(int argc, const char* argv[]) {
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* bitmapTex = NULL;
	int posX = 100, posY = 100, width = 640, height = 480;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	PHYSFS_init(argv[0]);
	PHYSFS_addToSearchPath(PHYSFS_getBaseDir(),1);

	win = SDL_CreateWindow("Hello World", posX, posY, width, height, SDL_WINDOW_RESIZABLE);

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, width, height);

	sago::SagoDataHolder holder(renderer);
	sago::SagoSpriteHolder spriteHolder(holder);

	const sago::SagoSprite& redSprite = spriteHolder.GetSprite("block_red");

	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
		for (int i=0; i< 5; ++i) {
			redSprite.Draw(renderer, 1, i*50, i*50);
		}
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
