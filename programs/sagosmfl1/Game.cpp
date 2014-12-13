/* 
 * File:   game.cpp
 * Author: poul
 * 
 * Created on 29. november 2014, 14:36
 */

#include "Game.hpp"
#include "model/Items.hpp"
#include <iostream>
#include "sago/SagoSprite.hpp"
#include "sago/SagoSpriteHolder.hpp"

using namespace std;

struct Game::GameImpl {
	vector<shared_ptr<Placeable> > placeables;
	const sago::SagoDataHolder *dataHolder;
	std::shared_ptr<sago::SagoSpriteHolder> sprites;
	float time = 0.0;
	shared_ptr<Human> human;
	shared_ptr<Tile> basic_tile;
	int center_x = 0;
	int center_y = 0;
};



Game::Game(const sago::SagoDataHolder &dataHolder) {
	data = new GameImpl();
	data->dataHolder = &dataHolder;
	data->sprites = std::shared_ptr<sago::SagoSpriteHolder>(new sago::SagoSpriteHolder(*(data->dataHolder)));
	shared_ptr<Human> human (new Human());
	data->placeables.push_back(human);
	data->human = human;
	data->basic_tile = shared_ptr<Tile> (new Tile());
	data->basic_tile->sprite = "terrain_grass_center";
}

Game::~Game() {
	delete data;
}

bool Game::IsActive() {
	return true;
}
bool Game::IsBlockingDraw()  {
	return true;
}
bool Game::IsBlockingUpdate()  {
	return true;
}

static void DrawHumanEntity(sf::RenderWindow &target, const std::shared_ptr<sago::SagoSpriteHolder> &sHolder, const Human *entity, float time, int offsetX, int offsetY) {
	string animation = "standing";
	if (entity->moving) {
		animation = "walkcycle";
	}
	const sago::SagoSprite &mySprite = sHolder->GetSprite(entity->race + "_"+animation+"_"+string(1,entity->direction));
	mySprite.Draw(target, time, entity->X-offsetX, entity->Y-offsetY);
}

void Game::DrawTiles(sf::RenderWindow &target, int topX, int topY, int width, int height) {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			const sago::SagoSprite &sprite = data->sprites->GetSprite(data->basic_tile->sprite);
			sprite.Draw(target, data->time, topX+x*32, topY+y*32);
		}
	}
}

void Game::Draw(sf::RenderWindow &target) {
	DrawTiles(target, (-data->center_x)%32-32, (-data->center_y)%32-32, 40, 30);
	for (const auto& placeable : data->placeables) {
		const Human *h = dynamic_cast<Human*>(placeable.get());
		if (h) {
			DrawHumanEntity(target, data->sprites, h, data->time, data->center_x-1024/2, data->center_y-768/2);
		}
	}
}

static void MoveHumanEntity (Creature *entity, float directionX, float directionY, float fDeltaTime) {
	float deltaX = directionX;
	float deltaY = directionY;
	if (deltaX == 0.0f && deltaY == 0.0f) {
		entity->moving = false;
		return;
	}
	entity->moving = true;
	if (deltaX*deltaX+deltaY*deltaY > 1.5f) {
		deltaX *= 0.7071067811865476f; //sqrt(0.5)
		deltaY *= 0.7071067811865476f; //sqrt(0.5)
	}
	if (deltaY > 0.0f) {
		entity->direction = 'S';
	}
	if (deltaY < 0.0f) {
		entity->direction = 'N';
	}
	if (deltaX < 0.0f) {
		entity->direction = 'W';
	}
	if (deltaX > 0.0f) {
		entity->direction = 'E';
	}
	float speed = 4.0f;
	entity->X += deltaX*(fDeltaTime/speed);
	entity->Y += deltaY*(fDeltaTime/speed);
}

void Game::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	data->time += fDeltaTime;
	float deltaX = 0.0f;
	float deltaY = 0.0f;
	if (input.IsPressed("DOWN")) {
		deltaY += 1.0f;
	}
	if (input.IsPressed("UP")) {
		deltaY -= 1.0f;
	}
	if (input.IsPressed("LEFT")) {
		deltaX -= 1.0f;
	}
	if (input.IsPressed("RIGHT")) {
		deltaX += 1.0f;
	}
	MoveHumanEntity(data->human.get(), deltaX, deltaY, fDeltaTime);
	data->center_x = data->human->X;
	data->center_y = data->human->Y;
}

void Game::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	
}