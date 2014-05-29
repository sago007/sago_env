/* 
 * File:   TextureHolder.hpp
 * Author: poul
 *
 * Created on 29. maj 2014, 14:05
 */

#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>

#ifndef TEXTUREHOLDER_HPP
#define	TEXTUREHOLDER_HPP

class TextureHolder {
public:
	TextureHolder();
	const sf::Texture* getTexturePtr(const std::string &textureName) const;
	virtual ~TextureHolder();
private:
	mutable std::unordered_map<std::string,sf::Texture*> textures;
};

#endif	/* TEXTUREHOLDER_HPP */

