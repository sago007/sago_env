/* 
 * File:   onemap.hpp
 * Author: poul
 *
 * Created on 17. maj 2015, 13:35
 */

#ifndef ONEMAP_HPP
#define	ONEMAP_HPP

#include <set>
#include <string>

struct OneMap {
	std::string name;
	std::string longname;
	std::string special;
	std::set<std::string> bots;
	int fraglimit = 0;
	int timelimit = 0;
	std::set<std::string> gametype;
};

#endif	/* ONEMAP_HPP */

