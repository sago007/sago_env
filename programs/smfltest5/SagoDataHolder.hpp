/* 
 * File:   TextureHolder.hpp
 * Author: poul
 *
 * Created on 29. maj 2014, 14:05
 */

#include <SFML/Graphics.hpp>

#ifndef TEXTUREHOLDER_HPP
#define	TEXTUREHOLDER_HPP

class SagoDataHolder {
public:
	SagoDataHolder();
	const sf::Texture* getTexturePtr(const std::string &textureName) const;
	const sf::Font* getFontPtr(const std::string &fontName) const;
	virtual ~SagoDataHolder();
private:
	struct SagoDataHolderData;
	mutable SagoDataHolderData *data;
};

#endif	/* TEXTUREHOLDER_HPP */

