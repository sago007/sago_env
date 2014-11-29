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

#ifndef SAGOSPRITE_HPP
#define	SAGOSPRITE_HPP

#include "SagoDataHolder.hpp"

namespace sago {

class SagoSprite {
public:
	SagoSprite(const SagoDataHolder &texHolder, const std::string &texture,const sf::IntRect &initImage,const int animationFrames, const int animationFrameLength);
	/**
	 * Draws the sprite to a given render window
     * @param target The render window to draw on
     * @param frameTime The time in milliseonds since gamestart. Used to determen the place in the animation
     * @param x Place to draw the sprite
     * @param y Place to draw the sprite
     */
	void Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y) const;
	/**
	 * Draws part of the sprite to a given render window
     * @param target The render window to draw on
     * @param frameTime The time in milliseonds since gamestart. Used to determen the place in the animation
     * @param x Place to draw the sprite
     * @param y Place to draw the sprite
     * @param part the part of the sprite that should be drawn. 
     */
	void Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y,const sf::IntRect &part) const;
	/**
	 * Set a different origin. Normally it is the top left cornor. But in some cases you might want to center the origin or tranform it for other reasons
     * @param newOrigin the coordinates that should be the new origin. Call with {0,0} to reset to default 
     */
	void SetOrigin(const sf::Vector2i &newOrigin);
	virtual ~SagoSprite();
private:
	SagoSprite(const SagoSprite& base) = delete;
    SagoSprite& operator=(const SagoSprite& base) = delete;
	struct SagoSpriteData;
	SagoSpriteData *data;
};

}

#endif	/* SAGOSPRITE_HPP */

