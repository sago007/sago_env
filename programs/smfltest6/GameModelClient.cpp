/* 
 * File:   GameModelClient.cpp
 * Author: poul
 * 
 * Created on 30. maj 2014, 20:12
 */

#include "GameModelClient.hpp"

struct GameModelClient::GameModelClientData {
};

GameModelClient::GameModelClient() {
	data = new GameModelClient::GameModelClientData();
}

GameModelClient::~GameModelClient() {
	delete data;
}

