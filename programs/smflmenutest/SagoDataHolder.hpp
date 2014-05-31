/* 
 * File:   TextureHolder.hpp
 * Author: poul
 *
 * Created on 29. maj 2014, 14:05
 */

#include <SFML/Graphics.hpp>

#ifndef TEXTUREHOLDER_HPP
#define	TEXTUREHOLDER_HPP

namespace sago {

class SagoDataHolder {
public:
	SagoDataHolder();
	const sf::Texture* getTexturePtr(const std::string &textureName) const;
	const sf::Font* getFontPtr(const std::string &fontName) const;
	virtual ~SagoDataHolder();
private:
	SagoDataHolder(const SagoDataHolder& base) = delete;
    SagoDataHolder& operator=(const SagoDataHolder& base) = delete;
	struct SagoDataHolderData;
	mutable SagoDataHolderData *data;
};

} //namespace sago

#endif	/* TEXTUREHOLDER_HPP */

