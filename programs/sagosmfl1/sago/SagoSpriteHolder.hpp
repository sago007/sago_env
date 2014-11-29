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

#ifndef SAGOSPRITEHOLDER_HPP
#define	SAGOSPRITEHOLDER_HPP

#include "SagoDataHolder.hpp"
#include "SagoSprite.hpp"

namespace sago {

class SagoSpriteHolder {
public:
	SagoSpriteHolder(const SagoDataHolder &texHolder);
	virtual ~SagoSpriteHolder();
	void ReadSprites();
	const sago::SagoSprite& GetSprite(const std::string &spritename) const;
	const SagoDataHolder& GetDataHolder() const;
private:
	SagoSpriteHolder(const SagoSpriteHolder& base) = delete;
    SagoSpriteHolder& operator=(const SagoSpriteHolder& base) = delete;
	void ReadSpriteFile(const std::string &filename);
	struct SagoSpriteHolderData;
	SagoSpriteHolderData *data;
};

}

#endif	/* SAGOSPRITEHOLDER_HPP */

