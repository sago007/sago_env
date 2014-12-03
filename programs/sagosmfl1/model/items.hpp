/* 
 * File:   items.hpp
 * Author: poul
 *
 * Created on 2. december 2014, 17:47
 */

#ifndef ITEMS_HPP
#define	ITEMS_HPP

class Tile {
public:
	int internalId = 0;
	std::string name;
	std::string sprite;
	bool blocking = false;
};

class Placeable {
public:
	float X = 10.0;
	float Y = 20.0;
	float Radius = 0.0;
	virtual bool isStatic() {return true; }
	virtual const char* getType() { return ""; }
};

class Creature : public Placeable {
public:
	float health = 10.0;
	float stinema = 10.0;
	float mana = 10.0;
	virtual bool isStatic() override { return false; }
};

class Human : public Creature {
public:
	std::string race;
	char direction = 'S';
	virtual const char* getType() override { return "Human"; }
};

#endif	/* ITEMS_HPP */

