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

#include "SagoMisc.hpp"
#include <physfs.h>
#include <iostream>

using namespace std;

namespace sago {
	
	
	std::vector<std::string> GetFileList(const char* dir) {
		std::vector<std::string> ret;
		char **rc = PHYSFS_enumerateFiles(dir);
		for (char **i = rc; *i != NULL; i++) {
			ret.push_back(*i);
		}
		PHYSFS_freeList(rc);
		return ret;
	}
	
	std::string GetFileContent(const char* filename) {
		std::string ret;
		if (!PHYSFS_exists(filename)) {
			std::cerr << "GetFileContent - File does not exists: " << filename << std::endl;
			return ret;
		}
		PHYSFS_file* myfile = PHYSFS_openRead(filename);
		unsigned int m_size = PHYSFS_fileLength(myfile);
		char *m_data = new char[m_size+1];
		int length_read = PHYSFS_read (myfile, m_data, 1, m_size);
		if (length_read != (int)m_size) {
			delete [] m_data;
			m_data = nullptr;
			PHYSFS_close(myfile);
			std::cerr << "Error: Curropt data file: " << filename << std::endl;
			return ret;
		}
		PHYSFS_close(myfile);
		m_data[m_size] = 0; //ensure that we are 0 terminated
		ret = m_data;
		delete [] m_data;
		return ret;
	}
	
	void DrawText(sf::RenderWindow &target, sf::Text font, const std::string &text, float x, float y, unsigned int size, sf::Color color) {
		font.setCharacterSize(size);
		font.setColor(color);
		font.setPosition(x,y);
		font.setString(text);
		target.draw(font);
	}
	
}