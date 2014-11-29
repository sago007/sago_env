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

#include "FrameCounter.hpp"
#include <sstream>

namespace sago {

FrameCounter::FrameCounter(const SagoDataHolder &dataHolder) {
	fpstext.setFont(*dataHolder.getFontPtr("FreeSerif"));
	fpstext.setString("FPS");
	fpstext.setCharacterSize(24);
	fpstext.setColor(sf::Color::Red);
}

FrameCounter::~FrameCounter() {
}

void FrameCounter::Draw(sf::RenderWindow &target, sf::Int32 frameTime) {
	fpscount++;
	if ( frameTime > lastFPSupdate + 1000 ) {
		std::stringstream ss;
		ss << "FPS: " << fpscount;
		fpstext.setString(ss.str());
		lastFPSupdate = frameTime;
		fpscount = 0;
	}
	target.draw(fpstext);
}

}