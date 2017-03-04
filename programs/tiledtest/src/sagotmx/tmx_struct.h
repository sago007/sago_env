/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tmx_struct.h
 * Author: poul
 *
 * Created on 28. februar 2017, 19:52
 */

#ifndef TMX_STRUCT_H
#define TMX_STRUCT_H

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "rapidxml/rapidxml.hpp"

#include <string>
#include <vector>

struct Terrain {
	std::string name;
	std::string tile;
	
	template <class Archive>
	void serialize( Archive & ar ) { ar( CEREAL_NVP(name), CEREAL_NVP(tile) ); }
};

struct TerrainTypes {
	std::vector<Terrain> terrains;
	
	template <class Archive>
	void serialize( Archive & ar ) { ar( CEREAL_NVP(terrains) ); }
};

struct Tile {
	int id = 0;
	std::string terrain;
	std::string probability;
	
	template <class Archive>
	void serialize( Archive & ar ) { ar( CEREAL_NVP(id), CEREAL_NVP(terrain), CEREAL_NVP(probability) ); }
};

struct Image {
	std::string source;
	int width = 0;
	int height = 0;
	
	template <class Archive>
	void serialize( Archive & ar ) { ar( CEREAL_NVP(source), CEREAL_NVP(width), CEREAL_NVP(height) ); }
};


struct TileSet {
	std::string firstgid;
	std::string source;
	std::string name;
	int tilewidth = 0;
	int tileheight = 0;
	int spacing = 0;
	int margin = 0;
	int tilecount = 0;
	int columns = 0;
	Image image;
	std::vector<Terrain> terrainTypes;
	
	template <class Archive>
	void serialize( Archive & ar ) { ar( CEREAL_NVP(firstgid), CEREAL_NVP(source), CEREAL_NVP(name), CEREAL_NVP(tilewidth), CEREAL_NVP(tileheight), 
			CEREAL_NVP(spacing), CEREAL_NVP(margin), CEREAL_NVP(tilecount), CEREAL_NVP(columns), CEREAL_NVP(terrainTypes), CEREAL_NVP(image) ); }
};

inline void setValueFromAttribute(rapidxml::xml_node<> * node, const char* name, std::string& result) {
	auto val = node->first_attribute(name);
	if (val) {
		result = val->value();
	}
}

inline void setValueFromAttribute(rapidxml::xml_node<> * node, const char* name, int& result) {
	auto val = node->first_attribute(name);
	if (val) {
		result = std::stoi(val->value());
	}
}

inline rapidxml::xml_node<> * getElement(rapidxml::xml_node<> * node, const char* name) {
	auto val = node->first_node(name);
	if (!val) {
		std::cerr << "Failed to find mandatory value: " << name << " on: " << node->name() << "\n";
		abort();
	}
	return val;
}

inline TileSet string2tileset(const std::string& tsx_content) {
	TileSet ts;
	std::string tsx_parseable_content = tsx_content;
	rapidxml::xml_document<> doc;    // character type defaults to char
	char* parsable_pointer = &tsx_parseable_content[0];  //Legal from C++11 and forward
	doc.parse<0>(parsable_pointer); 
	rapidxml::xml_node<> * root_node = doc.first_node("tileset");
	setValueFromAttribute( root_node, "name", ts.name);
	setValueFromAttribute( root_node, "source", ts.source);
	setValueFromAttribute( root_node, "tilewidth", ts.tilewidth);
	setValueFromAttribute( root_node, "tileheight", ts.tileheight);
	setValueFromAttribute( root_node, "tilecount", ts.tilecount);
	//ts.name = root_node->first_attribute("name")->value();
	//ts.source = root_node->first_attribute("source")->value();
	rapidxml::xml_node<> * image_node = getElement(root_node, "image");
	setValueFromAttribute(image_node, "source", ts.image.source);
	setValueFromAttribute(image_node, "width", ts.image.width);
	setValueFromAttribute(image_node, "height", ts.image.height);
	return ts;
}

#endif /* TMX_STRUCT_H */

