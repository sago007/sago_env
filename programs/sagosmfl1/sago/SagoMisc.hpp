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

#ifndef SAGOMISC_HPP
#define	SAGOMISC_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace sago {
	
	/**
	 * Returns a vector with all filenames in a given directory.
	 * PHYSFS must be setup before hand. The directory is relative to the PHYSFS base
     * @param dir The directory to list
     * @return A vector with the filenames in the given directory. If empty the directory was empty or did not exist
     */
	std::vector<std::string> GetFileList(const char* dir);
	
	/**
	 * Reads an entire file into memory.
	 * PHYSFS must be setup before hand
     * @param filename The file to read
     * @return The content of the file. If empty either the file was empty, did not exist or could not be opened
     */
	std::string GetFileContent(const char* filename);
	
	void DrawText(sf::RenderWindow &target, sf::Text font, const std::string &text, float x, float y, unsigned int size, sf::Color color = sf::Color::White);
		
}  //namespace sago

#endif	/* SAGOMISC_HPP */

