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

#ifndef SAGOMISC_HPP
#define	SAGOMISC_HPP

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
	
	bool FileExists(const char* filename);
	
	void WriteFileContent(const char* filename, const std::string& content);
	
	std::string GetHomeFolder( const std::string& gamename);
	void CreateHomeFolder( const std::string& gamename);
		
}  //namespace sago

#endif	/* SAGOMISC_HPP */

