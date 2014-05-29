/* 
 * File:   FrameCounter.hpp
 * Author: poul
 *
 * Created on 29. maj 2014, 22:23
 */

#ifndef FRAMECOUNTER_HPP
#define	FRAMECOUNTER_HPP

#include "SagoDataHolder.hpp"

class FrameCounter {
public:
	FrameCounter(const SagoDataHolder &dataHolder);
	virtual ~FrameCounter();
	void Draw(sf::RenderWindow &target, sf::Int32 frameTime);
private:
	sf::Text fpstext;
	sf::Int32 lastFPSupdate = 0;
	int fpscount = 0;

};

#endif	/* FRAMECOUNTER_HPP */

