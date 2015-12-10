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
	SDL_Texture* tex;
	SDL_Rect imgCord;
	SDL_Rect origin = {};
	int aniFrames = 0;
	int aniFrameTime = 0;
};

SagoSprite::SagoSprite(const SagoDataHolder& texHolder, const std::string& texture,const SDL_Rect& initImage,const int animationFrames, const int animationFrameLength) {
	data = new SagoSpriteData();
	data->tex = texHolder.getTexturePtr(texture);
	data->imgCord = initImage;
	data->aniFrames = animationFrames;
	data->aniFrameTime = animationFrameLength;
}



SagoSprite::~SagoSprite() {
	delete data;
}

void SagoSprite::Draw(SDL_Renderer* target, Sint32 frameTime, float x, float y) const {
	SDL_Rect rect = data->imgCord;
	rect.x+=rect.w*((frameTime/data->aniFrameTime)%data->aniFrames);
	SDL_Rect pos = rect;
	pos.x = x;
	pos.y = y;
	SDL_RenderCopy(target, data->tex, &rect, &pos);
}

void SagoSprite::Draw(SDL_Renderer* target, Sint32 frameTime, float x, float y, const SDL_Rect& part) const {
	SDL_Rect rect = data->imgCord;
	rect.x+=rect.w*((frameTime/data->aniFrameTime)%data->aniFrames);
	rect.x += part.x;
	rect.y += part.y;
	rect.w = part.w;
	rect.h = part.h;
	SDL_Rect pos = rect;
	pos.x = x;
	pos.y = y;
	SDL_RenderCopy(target, data->tex, &rect, &pos);
}

void SagoSprite::SetOrigin(const SDL_Rect& newOrigin) {
	data->origin = newOrigin;
}

}  //namespace sago