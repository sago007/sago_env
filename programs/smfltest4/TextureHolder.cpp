/* 
 * File:   TextureHolder.cpp
 * Author: poul
 * 
 * Created on 29. maj 2014, 14:05
 */

#include "TextureHolder.hpp"
#include <iostream>
#include <mutex>
#include <physfs.h>  

namespace {
std::mutex mutex_texture_load;
}  //anonymous namespace

TextureHolder::TextureHolder() {
}

TextureHolder::~TextureHolder() {
}

const sf::Texture* TextureHolder::getTexturePtr(const std::string &textureName) const {
	std::lock_guard<std::mutex> guard(mutex_texture_load);
	sf::Texture *ret = textures[textureName];
	if (ret) {
		return ret;
	}
	else {
		ret = new sf::Texture();
	}
	std::string path = "data/textures/"+textureName+".png";
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
	//Note: loadFromMemory assumes that the memory keeps being availeble. Therefore we do not free m_data
	bool success = ret->loadFromMemory(m_data,m_size);
	if (!success) {
		std::cerr << "getTextureFailed to load " << path << std::endl;
	}
	std::cout << path << " loaded" << std::endl;
	textures[textureName] = ret;
	return ret;
}
