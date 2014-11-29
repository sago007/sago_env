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

#include "SagoCommandQueue.hpp"
#include <vector>
#include <map>
#include <unordered_map>
#include <SFML/Window/Keyboard.hpp>


namespace sago {

struct BoundCommand {
	std::string bindname;
	std::string cmd;
	bool lastState = false;
};
	
struct SagoCommandQueue::SagoCommandQueueData {
	std::vector<std::string> queue;
	std::unordered_map<std::string,bool> keys;
	sf::Vector2i mousePosition = sf::Vector2i(0, 0);
	bool mouseMoved = false;
	std::map<sf::Keyboard::Key, std::string> binds;
	std::map<sf::Mouse::Button, std::string> mouseBinds;
	std::vector<BoundCommand> bindCommands;
};	
	
SagoCommandQueue::SagoCommandQueue() {
	data = new SagoCommandQueue::SagoCommandQueueData();
}

SagoCommandQueue::~SagoCommandQueue() {
	delete data;
}


void SagoCommandQueue::ReadKeysAndAddCommands(sf::RenderWindow &window) {
	for (auto iterator = data->binds.begin(); iterator != data->binds.end(); iterator++) {
		data->keys[iterator->second] = sf::Keyboard::isKeyPressed(iterator->first);
	}
	for (BoundCommand &item : data->bindCommands) {
		bool pressed = data->keys[item.bindname];
		if (pressed && !item.lastState) {
			data->queue.push_back(item.cmd);
		}
		item.lastState = pressed;
	}
	sf::Vector2i localPosition = sf::Mouse::getPosition(window);
	this->data->mouseMoved = (localPosition != this->data->mousePosition);
	this->data->mousePosition = localPosition;
}

void SagoCommandQueue::PushCommand(const std::string& cmd) {
	data->queue.push_back(cmd);
}

void SagoCommandQueue::ClearCommands() {
	data->queue.clear();
}

void SagoCommandQueue::BindKey(const sf::Keyboard::Key& key, const std::string& bindname) {
	data->binds[key] = bindname;
}

void SagoCommandQueue::BindMouseButton(const sf::Mouse::Button &b, const std::string &bindname) {
	data->mouseBinds[b] = bindname;
}

void SagoCommandQueue::BindKeyCommand(const std::string &bindname, const std::string& cmd) {
	BoundCommand bc;
	bc.bindname = bindname;
	bc.cmd = cmd;
	data->bindCommands.push_back(bc);
}

bool SagoCommandQueue::IsPressed(const std::string& bindname) const {
	return data->keys[bindname];
}

bool SagoCommandQueue::MouseMoved() const {
	return data->mouseMoved;
}


const std::vector<std::string> &SagoCommandQueue::GetCommandQueue() const {
	return data->queue;
}

const sf::Vector2i &SagoCommandQueue::GetMousePosition() const {
	return data->mousePosition;
}

} //namespace sago
