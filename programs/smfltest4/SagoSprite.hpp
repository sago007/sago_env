/* 
 * File:   SagoSprite.hpp
 * Author: poul
 *
 * Created on 29. maj 2014, 14:51
 */

#ifndef SAGOSPRITE_HPP
#define	SAGOSPRITE_HPP

#include "TextureHolder.hpp"

class SagoSprite {
public:
	SagoSprite(const TextureHolder &texHolder, const std::string &texture,const sf::IntRect &initImage,const int animationFrames, const int animationFrameLength);
	void Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y);
	virtual ~SagoSprite();
private:
	struct SagoSpriteData;
	SagoSpriteData *data;
};

#endif	/* SAGOSPRITE_HPP */

