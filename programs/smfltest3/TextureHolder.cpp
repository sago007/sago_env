/* 
 * File:   TextureHolder.cpp
 * Author: poul
 * 
 * Created on 29. maj 2014, 14:05
 */

#include "TextureHolder.hpp"
#include <iostream>
#include <mutex>

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
	bool success = ret->loadFromFile("data/textures/"+textureName+".png");
	if (!success) {
		std::cerr << "getTextureFailed to load " << textureName << std::endl;
	}
	textures[textureName] = ret;
	return ret;
}
