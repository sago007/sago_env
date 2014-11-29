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

#include "GameStateManager.hpp"

using namespace std;

namespace sago {

struct GameStateManager::GameStateManagerData {
	vector<shared_ptr<GameState>  > states;  
};

GameStateManager::GameStateManager() {
	this->data = new GameStateManager::GameStateManagerData();
}

GameStateManager::~GameStateManager() {
	delete this->data;
}

void GameStateManager::PushState(std::shared_ptr<GameState> s) {
	data->states.push_back(s);
}

void GameStateManager::PopInactive() {
	if (data->states.size() == 0) return;
	if (!data->states.back()->IsActive()) {
		data->states.pop_back();
		PopInactive();
	}
}

void GameStateManager::Draw(sf::RenderWindow &target) {
	if (data->states.size() == 0) return;
	size_t firstState = 0;
	for (size_t i = data->states.size(); i > firstState; i--) {
		if (data->states.at(i-1)->IsBlockingDraw() ) {
			firstState = i-1;
		}
	}
	for (size_t i = firstState; i < data->states.size(); i++) {
		data->states.at(i)->Draw(target);
	}
}

void GameStateManager::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	if (data->states.size() == 0) return;
	PopInactive();
	size_t firstState = 0;
	for (size_t i = data->states.size(); i > firstState; i--) {
		if (data->states.at(i-1)->IsBlockingUpdate() ) {
			firstState = i-1;
		}
	}
	for (size_t i = firstState; i < data->states.size(); i++) {
		data->states.at(i)->Update(fDeltaTime,input);
	}
}

void GameStateManager::UpdateCommandQueue(sago::SagoCommandQueue &inout) {
	if (data->states.size() == 0) return;
	size_t firstState = 0;
	for (size_t i = data->states.size(); i > firstState; i--) {
		if (data->states.at(i-1)->IsBlockingUpdate() ) {
			firstState = i-1;
		}
	}
	for (size_t i = firstState; i < data->states.size(); i++) {
		data->states.at(i)->UpdateCommandQueue(inout);
	}
}

}  //namespace sago