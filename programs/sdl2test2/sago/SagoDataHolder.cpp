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
#include <unordered_map>
#include <vector>
#include <physfs.h>
#include <SDL_mixer.h>  

namespace sago {

struct SagoDataHolder::SagoDataHolderData {
	std::unordered_map<std::string, SDL_Texture*> textures;
	//std::unordered_map<std::string, sf::Font*> fonts;
	std::unordered_map<std::string, Mix_Music*> music;
	std::unordered_map<std::string, Mix_Chunk*> sounds;
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
	delete data;
}

SDL_Texture* SagoDataHolder::getTexturePtr(const std::string &textureName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	SDL_Texture *ret = data->textures[textureName];
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
	SDL_RWops *rw = SDL_RWFromMem (m_data, m_size);
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

/*
const sf::Font* SagoDataHolder::getFontPtr(const std::string &fontName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	sf::Font *ret = data->fonts[fontName];
	if (ret) {
		return ret;
	}
	else {
		ret = new sf::Font();
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
	//Note: loadFromMemory assumes that the memory keeps being availeble. Therefore we do not free m_data
	bool success = ret->loadFromMemory(m_data,m_size);
	if (!success) {
		std::cerr << "getFontPtr to load " << path << std::endl;
	}
	std::cout << path << " loaded" << std::endl;
	data->fonts[fontName] = ret;
	return ret;
}
 */

Mix_Music* SagoDataHolder::getMusicPtr(const std::string &musicName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	Mix_Music *ret = data->music[musicName];
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
	SDL_RWops *rw = SDL_RWFromMem (m_data, m_size);

    //The above might fail an return null.
    if(!rw)
    {
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


Mix_Chunk* SagoDataHolder::getSoundPtr(const std::string &soundName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	Mix_Chunk *ret = data->sounds[soundName];
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
	SDL_RWops *rw = SDL_RWFromMem (m_data, m_size);

    //The above might fail an return null.
    if(!rw)
    {
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