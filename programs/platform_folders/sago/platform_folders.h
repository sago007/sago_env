#ifndef PLATFORM_FOLDERS_H
#define	PLATFORM_FOLDERS_H

#include <vector>
#include <string>

namespace sago {

std::string GetDataHome();
std::string GetConfigHome();
std::string GetCacheDir();
void AppendAdditionalDataDirectories(std::vector<std::string>& homes);
void AppendAdditionalConfigDirectories(std::vector<std::string>& homes);

class PlatformFolders {
public:
	PlatformFolders();
	~PlatformFolders();
	std::string GetDesktopFolder() const;
	std::string GetDocumentsFolder() const;
	std::string GetPicturesFolder() const;
	std::string GetDownloadFolder() const;
	std::string GetMusicFolder() const;
	std::string GetVideoFolder() const;
	std::string GetSaveGamesFolder1() const;
private:
	PlatformFolders(const PlatformFolders&);  
	PlatformFolders& operator=(const PlatformFolders&);
	struct PlatformFoldersData;
	mutable PlatformFoldersData *data;
};

}  //namespace sago

#endif	/* PLATFORM_FOLDERS_H */

