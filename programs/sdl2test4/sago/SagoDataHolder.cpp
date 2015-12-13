/*
===========================================================================
blockattack - Block Attack - Rise of the Blocks
Copyright (C) 2014 Poul Sander

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

Source information and contacts persons can be found at
http://blockattack.sf.net
===========================================================================
*/

#include "SagoDataHolder.hpp"
#include <iostream>
#include <mutex>
#include <map>
#include <vector>
#include <physfs.h>
#include <SDL_mixer.h>

namespace sago {

struct SagoDataHolder::SagoDataHolderData {
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, std::map<int, TTF_Font*> > fonts;  //font, ptsize
	std::map<std::string, Mix_Music*> music;
	std::map<std::string, Mix_Chunk*> sounds;
	std::vector<SDL_RWops*> rwOpsToFree;
	std::vector<char*> dataToFree;
	SDL_Renderer* renderer = nullptr;
};

namespace {
std::mutex mutex_texture_load;
}  //anonymous namespace

SagoDataHolder::SagoDataHolder(SDL_Renderer* renderer) {
	data = new SagoDataHolderData();
	data->renderer = renderer;
}

SagoDataHolder::~SagoDataHolder() {
	for (auto& item : data->textures) {
		SDL_DestroyTexture(item.second);
	}
	for (auto& item : data->music) {
		Mix_FreeMusic(item.second);
	}
	for (auto& item : data->sounds) {
		Mix_FreeChunk(item.second);
	}
	for (auto& item : data->fonts) {
		for (auto& item2 : item.second) {
			TTF_CloseFont(item2.second);
		}
	}
	delete data;
}

SDL_Texture* SagoDataHolder::getTexturePtr(const std::string& textureName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	SDL_Texture* ret = data->textures[textureName];
	if (ret) {
		return ret;
	}
	std::string path = "textures/"+textureName+".png";
	if (!PHYSFS_exists(path.c_str())) {
		std::cerr << "getTextureFailed - Texture does not exists: " << path << std::endl;
		return ret;
	}
	PHYSFS_file* myfile = PHYSFS_openRead(path.c_str());
	unsigned int m_size = PHYSFS_fileLength(myfile);
	char* m_data = new char[m_size];
	int length_read = PHYSFS_read (myfile, m_data, 1, m_size);
	if (length_read != (int)m_size) {
		delete [] m_data;
		m_data = 0;
		PHYSFS_close(myfile);
		std::cerr << "Error: Curropt data file: " << path << std::endl;
		return ret;
	}
	PHYSFS_close(myfile);
	SDL_RWops* rw = SDL_RWFromMem (m_data, m_size);
	//The above might fail an return null.
	if (!rw) {
		delete [] m_data;
		m_data = 0;
		PHYSFS_close(myfile);
		std::cerr << "Error. Curropt data file!" << std::endl;
		return NULL;
	}
	SDL_Surface* surface = IMG_Load_RW(rw,true);

	ret = SDL_CreateTextureFromSurface(data->renderer, surface);

	if (!ret) {
		std::cerr << "getTextureFailed to load " << path << std::endl;
	}
	SDL_FreeSurface(surface);
	delete [] m_data;
	std::cerr << path << " loaded" << std::endl;
	data->textures[textureName] = ret;
	return ret;
}

TTF_Font* SagoDataHolder::getFontPtr(const std::string &fontName, int ptsize) const {
    std::lock_guard<std::mutex> guard(mutex_texture_load);
    TTF_Font *ret = data->fonts[fontName][ptsize];
    if (ret) {
        return ret;
    }
    std::string path = "fonts/"+fontName+".ttf";
    if (!PHYSFS_exists(path.c_str())) {
        std::cerr << "getFontPtr - Font does not exists: " << path << std::endl;
        return ret;
    }
    PHYSFS_file* myfile = PHYSFS_openRead(path.c_str());
    unsigned int m_size = PHYSFS_fileLength(myfile);
    char *m_data = new char[m_size];
    int length_read = PHYSFS_read (myfile, m_data, 1, m_size);
    if (length_read != (int)m_size) {
        delete [] m_data;
        m_data = 0;
        PHYSFS_close(myfile);
        std::cerr << "Error: Curropt data file: " << path << std::endl;
        return ret;
    }
    PHYSFS_close(myfile);
	
	SDL_RWops* rw = SDL_RWFromMem (m_data, m_size);

	//The above might fail an return null.
	if (!rw) {
		delete [] m_data;
		m_data = 0;
		PHYSFS_close(myfile);
		std::cerr << "Error: Curropt data file!" << std::endl;
		return ret;
	}

	ret = TTF_OpenFontRW(rw, SDL_FALSE, ptsize);
	//delete [] m_data;
	if (!ret) {
		std::cerr << "Error openening font: " << fontName << " because: " << TTF_GetError() << std::endl;
	}
	data->fonts[fontName][ptsize] = ret;
	return ret;
}

Mix_Music* SagoDataHolder::getMusicPtr(const std::string& musicName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	Mix_Music* ret = data->music[musicName];
	if (ret) {
		return ret;
	}
	std::string path = "music/"+musicName+".ogg";
	if (!PHYSFS_exists(path.c_str())) {
		std::cerr << "getMusicPtr - Music file does not exists: " << path << std::endl;
		return ret;
	}
	PHYSFS_file* myfile = PHYSFS_openRead(path.c_str());
	unsigned int m_size = PHYSFS_fileLength(myfile);
	char* m_data = new char[m_size];
	int length_read = PHYSFS_read (myfile, m_data, 1, m_size);
	if (length_read != (int)m_size) {
		delete [] m_data;
		m_data = 0;
		PHYSFS_close(myfile);
		std::cerr << "Error: Curropt data file: " << path << std::endl;
		return ret;
	}
	PHYSFS_close(myfile);
	SDL_RWops* rw = SDL_RWFromMem (m_data, m_size);

	//The above might fail an return null.
	if (!rw) {
		delete [] m_data;
		m_data = 0;
		PHYSFS_close(myfile);
		std::cerr << "Error. Curropt data file!" << std::endl;
		return NULL;
	}

	ret = Mix_LoadMUS_RW(rw, SDL_TRUE);  //SDL_TRUE causes rw to be freed
	delete [] m_data;

	if (!ret) {
		std::cerr << "getMusicPtr to load " << path << std::endl;
	}
	std::cout << path << " loaded" << std::endl;
	data->music[musicName] = ret;
	return ret;
}


Mix_Chunk* SagoDataHolder::getSoundPtr(const std::string& soundName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	Mix_Chunk* ret = data->sounds[soundName];
	if (ret) {
		return ret;
	}
	std::string path = "sounds/"+soundName+".ogg";
	if (!PHYSFS_exists(path.c_str())) {
		std::cerr << "getSoundPtr - Sound file does not exists: " << path << std::endl;
		return ret;
	}
	PHYSFS_file* myfile = PHYSFS_openRead(path.c_str());
	unsigned int m_size = PHYSFS_fileLength(myfile);
	char* m_data = new char[m_size];
	int length_read = PHYSFS_read (myfile, m_data, 1, m_size);
	if (length_read != (int)m_size) {
		delete [] m_data;
		m_data = 0;
		PHYSFS_close(myfile);
		std::cerr << "Error: Curropt data file: " << path << std::endl;
		return ret;
	}
	PHYSFS_close(myfile);
	SDL_RWops* rw = SDL_RWFromMem (m_data, m_size);

	//The above might fail an return null.
	if (!rw) {
		delete [] m_data;
		m_data = 0;
		PHYSFS_close(myfile);
		std::cerr << "Error. Curropt data file!" << std::endl;
		return NULL;
	}

	ret = Mix_LoadWAV_RW(rw, SDL_TRUE);
	delete [] m_data;

	std::cout << path << " loaded" << std::endl;
	data->sounds[soundName] = ret;
	return ret;
}

} //name space sago
