/* 
 * File:   GameCharacter.hpp
 * Author: poul
 *
 * Created on 29. maj 2014, 21:20
 */

#ifndef GAMECHARACTER_HPP
#define	GAMECHARACTER_HPP

#include "MoveInput.hpp"
#include <string>

class GameCharacter {
public:
	GameCharacter();
	virtual ~GameCharacter();
	void Update(MoveInput &input, const float deltaFrameTime);
	
	std::string race;
	float x;
	float y;
	bool moving;
	int direction;  //< 0:Up, 1: Left, 2: Down, 3: Right
private:
	GameCharacter(const GameCharacter& base) = delete;
    GameCharacter& operator=(const GameCharacter& base) = delete;
};

#endif	/* GAMECHARACTER_HPP */

