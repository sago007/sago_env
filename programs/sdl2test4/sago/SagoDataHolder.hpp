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

#include "SDL.h"
#include <SDL_mixer.h>      //Used for sound & music
#include <SDL_image.h>      //To load PNG images!
#include <SDL_ttf.h>
#include <physfs.h>         //Abstract file system. To use containers
#include <string>

#ifndef TEXTUREHOLDER_HPP
#define	TEXTUREHOLDER_HPP

namespace sago {

class SagoDataHolder {
public:
	SagoDataHolder(SDL_Renderer* renderer);
	/**
	 * Return a pointer to the given texture. The pointer is valid for the lifetime of SagoDataHolder object it was taken from.
     * @param textureName Name of the texture
     * @return  Pointer to the loaded texture
     */
	SDL_Texture* getTexturePtr(const std::string &textureName) const;
	TTF_Font* getFontPtr(const std::string &fontName, int ptsize) const;
	Mix_Music* getMusicPtr(const std::string &musicName) const;
	Mix_Chunk* getSoundPtr(const std::string &soundName) const;
	virtual ~SagoDataHolder();
private:
	SagoDataHolder(const SagoDataHolder& base) = delete;
    SagoDataHolder& operator=(const SagoDataHolder& base) = delete;
	struct SagoDataHolderData;
	mutable SagoDataHolderData *data;
};

} //namespace sago

#endif	/* TEXTUREHOLDER_HPP */

