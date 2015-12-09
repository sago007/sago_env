#include <iostream>
#include "SDL.h"
#include <SDL_image.h>
#include "sago/SagoDataHolder.hpp"
#include <physfs.h>         //Abstract file system. To use containers


using namespace std;

int main(int argc, const char* argv[])
{
	SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bitmapTex = NULL;
    int posX = 100, posY = 100, width = 640, height = 480;

    SDL_Init(SDL_INIT_VIDEO);
	PHYSFS_init(argv[0]);
	PHYSFS_addToSearchPath(PHYSFS_getBaseDir(),1);

    win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	sago::SagoDataHolder holder(renderer);

	IMG_Init(IMG_INIT_PNG);
	bitmapTex = holder.getTexturePtr("red");

    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

}
