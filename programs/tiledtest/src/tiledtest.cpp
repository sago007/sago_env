#include <iostream>
#include <boost/program_options.hpp>
#include <SDL_mixer.h>
#include "sago/SagoDataHolder.hpp"
#include "sago/SagoSpriteHolder.hpp"
#include <sstream>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_rect.h>

#define CEREAL_XML_STRING_VALUE "tileset"
#include "sagotmx/tmx_struct.h"
#include "cereal/archives/xml.hpp"
#include "sago/SagoMisc.hpp"
#include "Libs/base64/base64.h"
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

static std::string string_decompress_decode(const std::string &data)
{
    std::stringstream compressed_encoded;
    std::stringstream decompressed;
    compressed_encoded << data;

    /** first decode  then decompress **/
    std::string compressed_str = base64_decode(compressed_encoded.str());
	std::stringstream compressed(compressed_str);

    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(compressed);
    boost::iostreams::copy(in, decompressed);
    return decompressed.str();
}

static void Draw(SDL_Renderer* target, SDL_Texture* t, int x, int y, const SDL_Rect& part) {
	SDL_Rect pos = {};
	pos.x = x;
	pos.y = y;
	pos.w = 32;
	pos.h = 32;
	SDL_RenderCopy(target, t, &part, &pos);
}

void runGame() {
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	int posX = 100, posY = 100, width = 640, height = 480;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	Mix_Init(MIX_INIT_OGG);
	
	win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	sago::SagoDataHolder holder(renderer);
	sago::SagoSpriteHolder spriteHolder(holder);
	std::string tsx_file = sago::GetFileContent("maps/Terrain.tsx");
	std::string tmx_file = sago::GetFileContent("maps/sample1.tmx");
	sago::tiled::TileSet ts = sago::tiled::string2tileset(tsx_file);
	sago::tiled::TileMap tm = sago::tiled::string2tilemap(tmx_file);
	std::string payload = tm.layers.at(1).data.payload;
	boost::trim(payload);
	std::cout << payload << "\n";
	std::vector<uint32_t> tiles;
	const unsigned char *data = reinterpret_cast<const unsigned char*>(string_decompress_decode(payload).data());
	unsigned tile_index = 0;
	for (int y = 0; y < tm.height; ++y) {
		for (int x = 0; x < tm.width; ++x) {
			uint32_t global_tile_id = data[tile_index] |
									  data[tile_index + 1] << 8 |
									  data[tile_index + 2] << 16 |
									  data[tile_index + 3] << 24;
			tile_index += 4;
			std::cout << global_tile_id << ", ";
			tiles.push_back(global_tile_id);
		}
	}
	/*{
		cereal::XMLOutputArchive archive( std::cout );
		ts.serialize(archive);
	}*/
	SDL_Texture* texture = holder.getTexturePtr("terrain");
	while (1) {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				break;
			}
		}

		SDL_RenderClear(renderer);
		for ( size_t i = 0; i < tiles.size(); ++i) {
			uint32_t gid = tiles.at(i);
			if (gid == 0) {
				continue;
			}
			gid-=1;   //first gid
			SDL_Rect part{};
			part.x = ( (gid) *ts.tilewidth)%ts.image.width;
			part.y = ( (gid) *ts.tilewidth)/ts.image.width* ts.tilewidth;
			part.h = 32;
			part.w = 32;
			Draw(renderer, texture, 32*(i%100), 32*(i/100), part);
		}
		
		usleep(10);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();

}

int main(int argc, const char* argv[]) {
	PHYSFS_init(argv[0]);
	PHYSFS_addToSearchPath((std::string(PHYSFS_getBaseDir())+"/data").c_str(), 1);
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
