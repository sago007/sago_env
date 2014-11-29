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

#include "SagoSprite.hpp"

namespace sago {

struct SagoSprite::SagoSpriteData {
	const sf::Texture* tex;
	sf::Sprite sprite;
	sf::IntRect imgCord;
	sf::Vector2i origin = sf::Vector2i(0,0);
	int aniFrames = 0;
	int aniFrameTime = 0;
};

SagoSprite::SagoSprite(const SagoDataHolder &texHolder, const std::string &texture,const sf::IntRect &initImage,const int animationFrames, const int animationFrameLength) {
	data = new SagoSpriteData();
	data->tex = texHolder.getTexturePtr(texture);
	data->sprite.setTexture(*texHolder.getTexturePtr(texture));
	data->imgCord = initImage;
	data->aniFrames = animationFrames;
	data->aniFrameTime = animationFrameLength;
}



SagoSprite::~SagoSprite() {
	delete data;
}

void SagoSprite::Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y) const {
	sf::IntRect rect = data->imgCord;
	rect.left+=rect.width*((frameTime/data->aniFrameTime)%data->aniFrames);
	data->sprite.setTextureRect(rect);
	data->sprite.setPosition(x-data->origin.x,y-data->origin.y);
	target.draw(data->sprite);
}

void SagoSprite::Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y, const sf::IntRect &part) const {
	sf::IntRect rect = data->imgCord;
	rect.left+=rect.width*((frameTime/data->aniFrameTime)%data->aniFrames);
	rect.left += part.left;
	rect.top += part.top;
	rect.width = part.width;
	rect.height = part.height;
	data->sprite.setTextureRect(rect);
	data->sprite.setPosition(x-data->origin.x,y-data->origin.y);
	target.draw(data->sprite);
}

void SagoSprite::SetOrigin(const sf::Vector2i &newOrigin) {
	data->origin = newOrigin;
}

}  //namespace sago