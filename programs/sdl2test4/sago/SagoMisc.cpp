/*
  Copyright (c) 2015 Poul Sander

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "SagoMisc.hpp"
#include <physfs.h>
#include <iostream>
#include <iconv.h>
#include <string.h>
#include <memory>

using namespace std;

namespace sago {


std::vector<std::string> GetFileList(const char* dir) {
	std::vector<std::string> ret;
	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** i = rc; *i != NULL; i++) {
		ret.push_back(*i);
	}
	PHYSFS_freeList(rc);
	return ret;
}

bool FileExists(const char* filename) {
	return PHYSFS_exists(filename);
}

std::string GetFileContent(const char* filename) {
	std::string ret;
	if (!PHYSFS_exists(filename)) {
		std::cerr << "GetFileContent - File does not exists: " << filename << std::endl;
		return ret;
	}
	PHYSFS_file* myfile = PHYSFS_openRead(filename);
	unsigned int m_size = PHYSFS_fileLength(myfile);
	char* m_data = new char[m_size+1];
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

void WriteFileContent(const char* filename, const std::string& content) {
	PHYSFS_file* myfile = PHYSFS_openWrite(filename);
	if (!myfile) {
		cerr << "Failed to open file for writing, " << PHYSFS_getLastError() << endl;
		return;
	}
	PHYSFS_write(myfile, content.c_str(), 1, content.length()+1);
	PHYSFS_close(myfile);
}

/*  void DrawText(sf::RenderWindow &target, sf::Text font, const std::string &text, float x, float y, unsigned int size, sf::Color color) {
        font.setCharacterSize(size);
        font.setColor(color);
        font.setPosition(x,y);
        font.setString(text);
        target.draw(font);
    }


    void Utf32ToUtf8( const sf::String& source, std::string &dest) {
        iconv_t ic;
        ic = iconv_open("UTF8", "UTF32");
        if (ic == 0) {
            dest = "iconv error";
            return;
        }
        size_t inSize = (source.getSize()+1)*sizeof(sf::Uint32);
        size_t outSize = inSize+1;
        std::unique_ptr<char[]> inBuffer(new char[inSize]());
        memcpy(static_cast<void*>(inBuffer.get()), source.getData(), source.getSize()*sizeof(sf::Uint32));
        std::unique_ptr<char[]> outBuffer(new char[outSize]());
        char* inPtr = inBuffer.get();
        char* outPtr = outBuffer.get();
        iconv(ic, &inPtr, &inSize, &outPtr, &outSize );
        iconv_close(ic);
        dest = outBuffer.get();
    }*/

std::string GetHomeFolder( const std::string& gamename) {
#if defined(__unix__)
	string home = (string)getenv("HOME")+(string)"/.gamesaves/"+gamename;
#elif defined(_WIN32)
	string home = (string)getMyDocumentsPath()+(string)"/My Games/"+gamename;
#endif
	return home;
}

void CreateHomeFolder( const std::string& gamename) {
#if defined(__unix__)
	//Compiler warns about unused result. The users envisonment should normally give the user all the information he needs
	string command = "mkdir -p \""+GetHomeFolder(gamename)+"\"";
	if (system( command.c_str())) {
		cerr << "command \"" << command << "\" failed!" << endl;
	}
#elif defined(_WIN32)
	//Now for Windows NT/2k/xp/2k3 etc.
	string tempA = getMyDocumentsPath()+"\\My Games";
	CreateDirectory(tempA.c_str(),NULL);
	tempA = getMyDocumentsPath()+(std::string)"\\My Games\\"+gamename;
	CreateDirectory(tempA.c_str(),NULL);
#endif

}

}