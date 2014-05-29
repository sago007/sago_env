/* 
 * File:   GameCharacter.cpp
 * Author: poul
 * 
 * Created on 29. maj 2014, 21:20
 */

#include "GameCharacter.hpp"



GameCharacter::GameCharacter() {
	x = 0.0f;
	y = 0.0f;
	direction = 2;
	moving = false;
}

GameCharacter::~GameCharacter() {
}

void GameCharacter::Update(MoveInput &input, const float deltaFrameTime) {
	float deltaY = 0.0f;
	float deltaX = 0.0f;
	moving = false;
	if (input.down) {
		direction = 2;
		deltaY += 1.0f;
	}
	if (input.up) {
		deltaY -= 1.0f;
		direction = 0;
	}
	if (input.left) {
		deltaX -= 1.0f;
		direction = 1;
	}
	if (input.right) {
		deltaX += 1.0f;
		direction = 3;
	}
	if (deltaX*deltaX+deltaY*deltaY > 1.5f) {
		deltaX *= 0.7071067811865476f; //sqrt(0.5)
		deltaY *= 0.7071067811865476f; //sqrt(0.5)
	}
	if (deltaX*deltaX > 0.1f || deltaY*deltaY > 0.1f) {
		moving = true;
	}
	float speed = 4.0f;
	x += deltaX*(deltaFrameTime/speed);
	y += deltaY*(deltaFrameTime/speed);
}

