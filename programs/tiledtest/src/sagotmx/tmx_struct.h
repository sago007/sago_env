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
	int tile;
	
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
	std::vector<Tile> tiles;
	std::map<int, Tile> tiles_map;
	
	template <class Archive>
	void serialize( Archive & ar ) { ar( CEREAL_NVP(firstgid), CEREAL_NVP(source), CEREAL_NVP(name), CEREAL_NVP(tilewidth), CEREAL_NVP(tileheight), 
			CEREAL_NVP(spacing), CEREAL_NVP(margin), CEREAL_NVP(tilecount), CEREAL_NVP(columns), CEREAL_NVP(terrainTypes), CEREAL_NVP(image), CEREAL_NVP(tiles) ); }
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
	rapidxml::xml_node<> * image_node = getElement(root_node, "image");
	setValueFromAttribute(image_node, "source", ts.image.source);
	setValueFromAttribute(image_node, "width", ts.image.width);
	setValueFromAttribute(image_node, "height", ts.image.height);
	rapidxml::xml_node<> * terraintypes_node = root_node->first_node("terraintypes");
	if (terraintypes_node) {
		for (rapidxml::xml_node<> * terrain_node = terraintypes_node->first_node("terrain"); terrain_node; terrain_node = terrain_node->next_sibling()) {
			Terrain t;
			setValueFromAttribute(terrain_node, "name", t.name);
			setValueFromAttribute(terrain_node, "tile", t.tile);
			ts.terrainTypes.push_back(t);
		}
	}
	for (rapidxml::xml_node<> * tile_node = root_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling()) {
		Tile t;
		setValueFromAttribute(tile_node, "id", t.id);
		setValueFromAttribute(tile_node, "terrain", t.terrain);
		setValueFromAttribute(tile_node, "probability", t.probability);
		ts.tiles_map[t.id] = t;
		ts.tiles.push_back(t);
	}
	return ts;
}

#endif /* TMX_STRUCT_H */

