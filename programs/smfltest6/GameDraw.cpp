/* 
 * File:   GameDraw.cpp
 * Author: poul
 * 
 * Created on 29. maj 2014, 21:49
 */

#include "GameDraw.hpp"

GameDraw::GameDraw() {
}

GameDraw::~GameDraw() {
}

void GameDraw::Draw(const GameCharacter &character,sf::Int32 frameTime) {
	if (character.moving) {
		SagoSprite spSprite(*texHolder,character.race+"_walkcycle",sf::IntRect(64,64*character.direction,64,64),8,100);
		spSprite.Draw(*render,frameTime,character.x,character.y);
	}
	else {
		SagoSprite spSpriteStand(*texHolder,character.race+"_walkcycle",sf::IntRect(0,64*character.direction,64,64),1,100);
		spSpriteStand.Draw(*render,frameTime,character.x,character.y);
	}
}