/* 
 * File:   GameDraw.hpp
 * Author: poul
 *
 * Created on 29. maj 2014, 21:49
 */

#ifndef GAMEDRAW_HPP
#define	GAMEDRAW_HPP

#include "GameCharacter.hpp"
#include "SagoSprite.hpp"

class GameDraw {
public:
	GameDraw();
	void Draw(const GameCharacter &character,sf::Int32 frameTime);
	virtual ~GameDraw();
	sf::RenderWindow *render;
	SagoDataHolder *texHolder;
	
private:
	GameDraw(const GameDraw& base) = delete;
    GameDraw& operator=(const GameDraw& base) = delete;
	
};

#endif	/* GAMEDRAW_HPP */

