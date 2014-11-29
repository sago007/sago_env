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

#ifndef FRAMECOUNTER_HPP
#define	FRAMECOUNTER_HPP

#include "SagoDataHolder.hpp"

namespace sago {

class FrameCounter {
public:
	/**
	 * Creates a framerate counter.
	 * The font "FreeSerif" will be taken from dataHolder
     * @param dataHolder The programs dataholder used the font. Must have a lifetime at least as long as the FrameCounter object
     */
	FrameCounter(const SagoDataHolder &dataHolder);
	virtual ~FrameCounter();
	/**
	 * Draws the framerate to the target
     * @param target The render window to draw on
     * @param frameTime time the draw was called in milliseconds since program start
     */
	void Draw(sf::RenderWindow &target, sf::Int32 frameTime);
private:
	FrameCounter(const FrameCounter& base) = delete;
    FrameCounter& operator=(const FrameCounter& base) = delete;
	sf::Text fpstext;
	sf::Int32 lastFPSupdate = 0;
	int fpscount = 0;

};

}

#endif	/* FRAMECOUNTER_HPP */

