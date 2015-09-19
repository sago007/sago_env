#include <iostream>
#include "sago/platform_folders.h"

using namespace std;
using namespace sago;

int main()
{
	cout << "Config: " << GetConfigHome() << endl;
	cout << "Data: " << GetDataHome() << endl;
	cout << "Cache: " << GetCacheDir() << endl;
	PlatformFolders p;
	cout << "Documents: " << p.GetDocumentsFolder() << endl;
	cout << "Desktop: " << p.GetDesktopFolder() << endl;
	cout << "Pictures: " << p.GetPicturesFolder() << endl;
	cout << "Music: " << p.GetMusicFolder() << endl;
	cout << "Video: " << p.GetVideoFolder() << endl;
	cout << "Download: " << p.GetDownloadFolder1() << endl;
	cout << "Save Games 1: " << p.GetSaveGamesFolder1() << endl;
	return 0;
}
