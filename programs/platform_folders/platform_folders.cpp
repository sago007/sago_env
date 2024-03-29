/*
  Its is under the MIT license, to encourage reuse by cut-and-paste.

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

#include <iostream>
#include "sago/platform_folders.h"

using std::cout;

int main()
{
	cout << "Config: " << sago::getConfigHome() << "\n";
	cout << "Data: " << sago::getDataHome() << "\n";
	cout << "Cache: " << sago::getCacheDir() << "\n";
	sago::PlatformFolders p;
	cout << "Documents: " << p.getDocumentsFolder() << "\n";
	cout << "Desktop: " << p.getDesktopFolder() << "\n";
	cout << "Pictures: " << p.getPicturesFolder() << "\n";
	cout << "Music: " << p.getMusicFolder() << "\n";
	cout << "Video: " << p.getVideoFolder() << "\n";
	cout << "Download: " << p.getDownloadFolder1() << "\n";
	cout << "Save Games 1: " << p.getSaveGamesFolder1() << "\n";
	return 0;
}
