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

#include "SagoSpriteHolder.hpp"
#include "SagoMisc.hpp"
#include <memory>
#include <unordered_map>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <string.h>
#include <boost/algorithm/string/predicate.hpp>


//I truely hate the C way of checking for equal. Usually read: "if not X compares to Y then they must be equal"
#define strequal(X,Y) strcmp(X,Y)==0

using namespace std;

namespace sago {

struct SagoSpriteHolder::SagoSpriteHolderData {
	const sago::SagoDataHolder* tex;
	std::unordered_map<std::string,std::shared_ptr<sago::SagoSprite>> sprites;
	const sago::SagoSprite* defaultSprite;
};
	
SagoSpriteHolder::SagoSpriteHolder(const SagoDataHolder &texHolder) {
	data = new SagoSpriteHolderData();
	try {
		data->tex = &texHolder;
		ReadSprites();
		data->defaultSprite = new sago::SagoSprite(texHolder,"fallback",sf::IntRect(0,0,64,64),1,100);
	} catch (...) {
		delete data;
	}
}

SagoSpriteHolder::~SagoSpriteHolder() {
	delete data;
}

void SagoSpriteHolder::ReadSpriteFile(const std::string &filename) {
	string fullfile = "sprites/"+filename;
	string content = sago::GetFileContent(fullfile.c_str());
	Json::Value root;   // will contains the root value after parsing
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( content, root );
	if ( !parsingSuccessful ) {
		cerr << "Failed to parse: " << fullfile << endl
				<< reader.getFormattedErrorMessages() << endl;
		return;
	}
	for (Json::Value::iterator it = root.begin(); it != root.end() ; ++it) {
		string spriteName = it.memberName();
		Json::Value value = (*it);
		string textureName = value.get("texture","fallback").asString();
		int topx = value.get("topx",0).asInt();
		int topy = value.get("topy",0).asInt();
		int height = value.get("height",0).asInt();
		int width = value.get("width",0).asInt();
		int number_of_frames = value.get("number_of_frames",1).asInt();
		int frame_time = value.get("frame_time",1).asInt();
		int originx = value.get("originx",0).asInt();
		int originy = value.get("originy",0).asInt();
		if (number_of_frames < 1) {
			number_of_frames = 1;
		}
		if (frame_time < 1) {
			frame_time = 1;
		}
		std::shared_ptr<sago::SagoSprite> ptr(new SagoSprite(*(data->tex),textureName,sf::IntRect(topx,topy,width,height),number_of_frames,frame_time));
		ptr->SetOrigin(sf::Vector2i(originx,originy));
		this->data->sprites[std::string(spriteName)] = ptr;
	}
}

void SagoSpriteHolder::ReadSprites() {
	std::vector<std::string> spritefiles = GetFileList("sprites");
	for (std::string &item : spritefiles  ) {
		if (boost::algorithm::ends_with(item,".sprite")) {
			cout << "Found " << item << endl;
			ReadSpriteFile(item);
		}
		else {
			cout << "Ignoreing " << item << endl;
		}
	}
}

const sago::SagoSprite& SagoSpriteHolder::GetSprite(const std::string &spritename) const {
	std::unordered_map<std::string,std::shared_ptr<sago::SagoSprite>>::const_iterator got = data->sprites.find (spritename);
	if ( got == data->sprites.end() ) {
		return *data->defaultSprite;
	}
	else {
		return *(got->second);
	}
}

const SagoDataHolder& SagoSpriteHolder::GetDataHolder() const {
	return *data->tex;
}

}