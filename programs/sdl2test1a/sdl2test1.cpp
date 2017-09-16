#include <iostream>
#include "SDL.h"
#include <SDL_image.h>


using namespace std;

SDL_Texture* CreateTextureFromFile(const char* filename, SDL_Renderer* renderer) {
	SDL_Surface* bitmapSurface = NULL;
	bitmapSurface = IMG_Load(filename);
	if (!bitmapSurface) {
		cerr << "Failed to load fallback.png - " << IMG_GetError() << endl;
	}
	SDL_Texture* ret = SDL_CreateTextureFromSurface(renderer, bitmapSurface);
	SDL_FreeSurface(bitmapSurface);
	return ret;
}

int main(int argc, const char* argv[]) {
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* bitmapTex[7] = {};

	int posX = 100, posY = 100, width = 640, height = 480;

	SDL_Init(SDL_INIT_VIDEO);

	win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	IMG_Init(IMG_INIT_PNG);
	bitmapTex[0] = CreateTextureFromFile("red.png", renderer);
	bitmapTex[1] = CreateTextureFromFile("blue.png", renderer);
	bitmapTex[2] = CreateTextureFromFile("green.png", renderer);
	bitmapTex[3] = CreateTextureFromFile("purple.png", renderer);
	bitmapTex[4] = CreateTextureFromFile("turkish.png", renderer);
	bitmapTex[5] = CreateTextureFromFile("yellow.png", renderer);
	bitmapTex[6] = CreateTextureFromFile("grey.png", renderer);


	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
		}

		SDL_RenderClear(renderer);
		SDL_Rect DestR;
		DestR.x = 100;
  		DestR.y = 100;
  		DestR.w = 50;
  		DestR.h = 50;
		srand(400);
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				DestR.x = i*50;
		  		DestR.y = j*50;
				SDL_RenderCopy(renderer, bitmapTex[rand()%7], NULL, &DestR);
			}
		}
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(bitmapTex[0]);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();

}
