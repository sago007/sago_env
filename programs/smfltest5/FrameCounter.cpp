/* 
 * File:   FrameCounter.cpp
 * Author: poul
 * 
 * Created on 29. maj 2014, 22:23
 */

#include "FrameCounter.hpp"
#include <sstream>

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
