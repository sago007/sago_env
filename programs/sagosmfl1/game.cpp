/* 
 * File:   game.cpp
 * Author: poul
 * 
 * Created on 29. november 2014, 14:36
 */

#include "game.hpp"
#include "model/items.hpp"
#include <iostream>
#include "sago/SagoSprite.hpp"
#include "sago/SagoSpriteHolder.hpp"

using namespace std;

struct Game::GameImpl {
	vector<shared_ptr<Placeable> > placeables;
	const sago::SagoDataHolder *dataHolder;
	std::shared_ptr<sago::SagoSpriteHolder> sprites;
	float time = 0.0;
};

Game::Game(const sago::SagoDataHolder &dataHolder) {
	data = new GameImpl();
	data->dataHolder = &dataHolder;
	data->sprites = std::shared_ptr<sago::SagoSpriteHolder>(new sago::SagoSpriteHolder(*(data->dataHolder)));
	shared_ptr<Human> human (new Human());
	data->placeables.push_back(human);
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
void Game::Draw(sf::RenderWindow &target) {
	for (const auto& placeable : data->placeables) {
		//cerr << placeable->X << endl;
		const sago::SagoSprite &mySprite = data->sprites->GetSprite("male_walkcycle_s");
		mySprite.Draw(target,data->time,placeable->X,placeable->Y);
	}
}

void Game::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	data->time += fDeltaTime;
}

void Game::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	
}