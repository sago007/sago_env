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

#include "rapidxml/rapidxml.hpp"

#include <string>
#include <vector>
#include <zlib.h>
#include <iostream>
#include "b64/decode.h"
#include "b64/encode.h"

namespace sago {
namespace tiled {

class SagoTiledException : public std::exception {
	char errMsg[256];
public:
	SagoTiledException() {
		errMsg[0] = '\0';
	}
	
	SagoTiledException(const char* fmt, ...) __attribute__ ((format (printf, 2, 3))) {
		va_list args;
		va_start(args, fmt);
		vsnprintf(errMsg, sizeof(errMsg), fmt, args);
		va_end(args);
	}
	
	virtual const char* what() const noexcept override {
		return errMsg;
	}

};

void* z_alloc(void *opaque __attribute__((unused)), unsigned int items, unsigned int size) {
	return realloc(NULL, items *size);
}

void z_free(void *opaque __attribute__((unused)), void *address) {
	return free(address);
}

//originally from Bayle Jonathan <baylej@github>'s lib. Needs to be even more C++ friendly
std::string zlib_decompress(const char *source, unsigned int slength, unsigned int rlength) {
	int ret;
	std::string res;
	z_stream strm;


	strm.zalloc = z_alloc;
	strm.zfree = z_free;
	strm.opaque = Z_NULL;
	strm.next_in = (Bytef*)source;
	strm.avail_in = slength;

	res.resize(rlength);

	strm.next_out = (Bytef*)&res[0];
	strm.avail_out = rlength;

	/* 15+32 to enable zlib and gzip decoding with automatic header detection */
	if ((ret=inflateInit2(&strm, 15 + 32)) != Z_OK) {
		std::cerr << "zlib_decompress: inflateInit2  " << ret << "\n"; 
		abort();
	}

	ret = inflate(&strm, Z_FINISH);
	inflateEnd(&strm);

	if (ret != Z_OK && ret != Z_STREAM_END) {
		std::cerr << "zlib_decompress: inflate returned " << ret << "\n"; 
		abort();
	}

	if (strm.avail_out != 0) {
		std::cerr << "layer contains not enough tiles\n"; 
		abort();
	}
	if (strm.avail_in != 0) {
		/* FIXME There is remains in the source */
	}

	return res;
}

inline std::string zlib_compress(const std::string& source) {
	std::string res;
	char buffer[1024];
	z_stream strm;
	
	strm.zalloc = z_alloc;
	strm.zfree = z_free;
	strm.opaque = Z_NULL;
	strm.next_in = (Bytef*)source.data();
	strm.avail_in = source.size();
	strm.next_out = (Bytef*)buffer;
	strm.avail_out = sizeof(buffer);
	
	int ret = deflateInit2(&strm, Z_BEST_SPEED,Z_DEFLATED, 15 | 16,
              8,
              Z_DEFAULT_STRATEGY);
	if (ret != Z_OK) {
		throw SagoTiledException("zlib error: deflateInit returned %d", ret);
	}
	while (ret == Z_OK) {
		ret = deflate(&strm, Z_FINISH);
		if (ret == Z_OK || ret == Z_STREAM_END) {
			res.append(buffer, sizeof(buffer));
			strm.next_out = (Bytef*)buffer;
			strm.avail_out = sizeof(buffer);
		}
	}
	if ( ret != Z_STREAM_END) {
		throw SagoTiledException("zlib error: deflate returned %d", ret);
	}
	res.resize(strm.total_out);
	
	deflateEnd(&strm);
	return res;
}

const char* const b64enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * This functions will skip whitespace until it finds a base64 encoding charecter. It will then decode it and decompress it.
 * @param data base64 encoded data of a zlib compressed array of gids_count uint32 elements.
 * @param gids_count Number of elements in the result.
 * @return The raw result. will be 4*gids_count large.
 */
inline std::string string_decompress_decode(const std::string &data, int gids_count)
{
	std::size_t found = data.find_first_of(b64enc);
	if (found == std::string::npos) {
		std::cerr << "Warning: Layer without data... this is most likely a mistake.\n";
		return "";
	}
	std::string compressed_str;
	compressed_str.resize(data.length());
	base64::decoder decoder;
	decoder.decode(&data[found], data.length()-found, &compressed_str[0]);
	return sago::tiled::zlib_decompress(compressed_str.c_str(), compressed_str.length(), (unsigned int)(gids_count*sizeof(int32_t)));
}

inline std::string string_encode( const std::string& data) {
	std::stringstream ret;
	std::stringstream input(data);
	base64::encoder E;
	E.encode(input, ret);
	return ret.str();
}

struct Terrain {
	std::string name;
	int tile;
};

struct TerrainTypes {
	std::vector<Terrain> terrains;
};

struct Tile {
	int id = 0;
	std::string terrain;
	std::string probability;
};

struct Image {
	std::string source;
	int width = 0;
	int height = 0;
};


struct TileSet {
	std::string firstgid;
	std::string source;
	TileSet* alternativeSource = nullptr;  //< Set to non-null to use an alternative TileSet. Must be set if source is set.
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
};

struct TileLayerData {
	std::string encoding; 
	std::string compression;
	std::string payload;
};

struct TileLayer {
	std::string name;
	int width=0;
	int height=0;
	TileLayerData data;
};

struct TileMap {
	std::string version;
	std::string orientation;
	std::string renderorder;
	int width = 0;
	int height = 0;
	int tilewidth=0; 
	int tileheight=0; 
	int nextobjectid = 0;
	TileSet tileset;
	std::vector<TileLayer> layers;
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
		throw new SagoTiledException("Failed to find mandatory valye: %s on %s", name, node->name());
	}
	return val;
}

inline rapidxml::xml_node<> * getElement(rapidxml::xml_node<> * node, const char* name, bool& found) {
	auto val = node->first_node(name);
	found  = val;
	return val;
}

inline TileSet node2tileset(rapidxml::xml_node<> * tileset_node) {
	TileSet ts;
	setValueFromAttribute( tileset_node, "name", ts.name);
	setValueFromAttribute( tileset_node, "source", ts.source);
	setValueFromAttribute( tileset_node, "tilewidth", ts.tilewidth);
	setValueFromAttribute( tileset_node, "tileheight", ts.tileheight);
	setValueFromAttribute( tileset_node, "tilecount", ts.tilecount);
	setValueFromAttribute( tileset_node, "firstgid", ts.firstgid);
	bool found;
	rapidxml::xml_node<> * image_node = getElement(tileset_node, "image", found);
	if (found) {
		setValueFromAttribute(image_node, "source", ts.image.source);
		setValueFromAttribute(image_node, "width", ts.image.width);
		setValueFromAttribute(image_node, "height", ts.image.height);
	}
	rapidxml::xml_node<> * terraintypes_node = tileset_node->first_node("terraintypes");
	if (terraintypes_node) {
		for (rapidxml::xml_node<> * terrain_node = terraintypes_node->first_node("terrain"); terrain_node; terrain_node = terrain_node->next_sibling()) {
			Terrain t;
			setValueFromAttribute(terrain_node, "name", t.name);
			setValueFromAttribute(terrain_node, "tile", t.tile);
			ts.terrainTypes.push_back(t);
		}
	}
	for (rapidxml::xml_node<> * tile_node = tileset_node->first_node("tile"); tile_node; tile_node = tile_node->next_sibling()) {
		Tile t;
		setValueFromAttribute(tile_node, "id", t.id);
		setValueFromAttribute(tile_node, "terrain", t.terrain);
		setValueFromAttribute(tile_node, "probability", t.probability);
		ts.tiles_map[t.id] = t;
		ts.tiles.push_back(t);
	}
	return ts;
}

inline TileSet string2tileset(const std::string& tsx_content) {
	std::string tsx_parseable_content = tsx_content;
	rapidxml::xml_document<> doc;    // character type defaults to char
	char* parsable_pointer = &tsx_parseable_content[0];  //Legal from C++11 and forward
	doc.parse<0>(parsable_pointer); 
	rapidxml::xml_node<> * root_node = doc.first_node("tileset");
	return node2tileset(root_node);
}

inline TileMap string2tilemap(const std::string& tmx_content) {
	TileMap m;
	std::string tmx_parseable_content = tmx_content;
	rapidxml::xml_document<> doc;    // character type defaults to char
	char* parsable_pointer = &tmx_parseable_content[0];  //Legal from C++11 and forward
	doc.parse<0>(parsable_pointer); 
	rapidxml::xml_node<> * root_node = doc.first_node("map");
	setValueFromAttribute( root_node, "verion", m.version);
	setValueFromAttribute( root_node, "orientation", m.orientation);
	setValueFromAttribute( root_node, "renderorder", m.renderorder);
	setValueFromAttribute( root_node, "width", m.width);
	setValueFromAttribute( root_node, "height", m.height);
	setValueFromAttribute( root_node, "tilewidth", m.tilewidth);
	setValueFromAttribute( root_node, "tileheight", m.tileheight);
	setValueFromAttribute( root_node, "nextobjectid", m.nextobjectid);
	auto tileset_node = getElement(root_node, "tileset");
	m.tileset = node2tileset(tileset_node);
	for (rapidxml::xml_node<> * layer_node = root_node->first_node("layer"); layer_node; layer_node = layer_node->next_sibling()) {
		TileLayer tl;
		setValueFromAttribute(layer_node, "name", tl.name);
		setValueFromAttribute(layer_node, "height", tl.height);
		setValueFromAttribute(layer_node, "width", tl.width);
		auto data_node = getElement(layer_node, "data");
		setValueFromAttribute(data_node, "encoding", tl.data.encoding);
		setValueFromAttribute(data_node, "compression", tl.data.compression);
		std::string compressed_payload = data_node->value();
		tl.data.payload = sago::tiled::string_decompress_decode(compressed_payload, m.height*m.width);
		m.layers.push_back(tl);
	}
	return m;
}

inline void xml_add_attribute(std::iostream& io, const std::string& name, const std::string& value) {
	if (value.length()) {
		io << " " << name << "=\"" << value << "\"";
	}
}

inline void xml_add_attribute(std::iostream& io, const char* name, int value) {
	if (value) {
		io << " " << name << "=\"" << value << "\"";
	}
}

inline void xml_add_tileset(std::iostream& io, const TileMap& m) {
	const auto& ts = m.tileset;
	io << "<tileset";
	xml_add_attribute(io, "firstgid", ts.firstgid);
	xml_add_attribute(io, "source", ts.source);
	io << "/>\n";
}

inline void xml_add_layer(std::iostream& io, const TileMap& m, size_t layer_number) {
	const TileLayer& l = m.layers.at(layer_number);
	io << "<layer";
	xml_add_attribute(io, "name", l.name);
	xml_add_attribute(io, "height", l.height);
	xml_add_attribute(io, "width", l.width);
	io << ">\n";
	io << "<data encoding=\"base64\" compression=\"zlib\">\n";
	io << string_encode(zlib_compress(l.data.payload));
	io << "</data>\n";
	io << "</layer>\n";
}

inline std::string tilemap2string(const TileMap& m) {
	//Assuming UTF-8 because rapidxml ignores it.
	std::stringstream ret;
	ret << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	ret << "<map version=\"1.0\"";
	xml_add_attribute(ret,  "orientation", m.orientation);
	xml_add_attribute(ret,  "renderorder", m.renderorder);
	xml_add_attribute(ret,  "width", m.width);
	xml_add_attribute(ret,  "height", m.height);
	xml_add_attribute(ret,  "tilewidth", m.tilewidth);
	xml_add_attribute(ret,  "tileheight", m.tileheight);
	xml_add_attribute(ret,  "nextobjectid", m.nextobjectid);
	ret << ">\n";
	xml_add_tileset(ret, m);
	for (size_t i = 0; i < m.layers.size(); ++i) {
		xml_add_layer(ret, m, i);
	}
	ret << "</map>\n";
	return ret.str();
}

inline void getTextureLocationFromGid(const TileMap& tm, int gid, std::string* imageFile, int* x, int* y, int* w, int* h ) {
	//Currently hardcoded to one tileset
	const TileSet *ts = &(tm.tileset);
	//TODO: Check this. Is "firstgid" from the tiledset related?
	gid--;  //Gids starts at 1 by default
	while (ts->alternativeSource) {
		//Follow source links
		ts = ts->alternativeSource;
	}
	if (imageFile) {
		*imageFile = ts->image.source;
	}
	if (x) {
		*x = ( gid *ts->tilewidth)%ts->image.width;
	}
	if (y) {
		*y = ( gid *ts->tilewidth)/ts->image.width* ts->tilewidth;
	}
	if (w) {
		*w = ts->tilewidth;
	}
	if (h) {
		*h = ts->tileheight;
	}
}

inline uint32_t getTileFromLayer(const TileMap& m, const TileLayer& l, int x, int y) {
	size_t tile_index = (m.height*y+x)*sizeof(uint32_t);
	const unsigned char *data = reinterpret_cast<const unsigned char*>(l.data.payload.data());
	uint32_t global_tile_id = data[tile_index] |
				data[tile_index + 1] << 8 |
				data[tile_index + 2] << 16 |
				data[tile_index + 3] << 24;
	return global_tile_id;
}

}  //tiled
}  //sago

#endif /* TMX_STRUCT_H */

