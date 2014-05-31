/* 
 * File:   SagoCommandQueue.cpp
 * Author: poul
 * 
 * Created on 31. maj 2014, 20:40
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
	std::map<sf::Keyboard::Key,std::string> binds;
	std::vector<BoundCommand> bindCommands;
};	
	
SagoCommandQueue::SagoCommandQueue() {
	data = new SagoCommandQueue::SagoCommandQueueData();
}

SagoCommandQueue::~SagoCommandQueue() {
	delete data;
}


void SagoCommandQueue::ReadKeysAndAddCommands() {
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

void SagoCommandQueue::BindKeyCommand(const std::string &bindname, const std::string& cmd) {
	BoundCommand bc;
	bc.bindname = bindname;
	bc.cmd = cmd;
	data->bindCommands.push_back(bc);
}

bool SagoCommandQueue::IsPressed(const std::string& bindname) const {
	return data->keys[bindname];
}

const std::vector<std::string> &SagoCommandQueue::GetCommandQueue() const {
	return data->queue;
}

}