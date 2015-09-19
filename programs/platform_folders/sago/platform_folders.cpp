#include "platform_folders.h"
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <stdio.h>
#include <cstdlib>

#if defined(_WIN32)
#include <windows.h>
#include <shlobj.h>

#define strtok_r strtok_s

static std::string GetWindowsFolder(int folderId, const char* errorMsg) {
    char szPath[MAX_PATH];
    szPath[0] = 0;
    if ( !SUCCEEDED( SHGetFolderPath( NULL, folderId, NULL, 0, szPath ) ) )
    {
        throw std::runtime_error(errorMsg);
    }
    return szPath;
}

static std::string GetAppData() {
    return GetWindowsFolder(CSIDL_APPDATA, "RoamingAppData could not be found");
}

static std::string GetAppDataCommon() {
    return GetWindowsFolder(CSIDL_COMMON_APPDATA, "Common appdata could not be found");
}

static std::string GetAppDataLocal() {
    return GetWindowsFolder(CSIDL_LOCAL_APPDATA, "LocalAppData could not be found");
}
#else
#include <map>
#include <fstream>
//Typically Linux. For easy reading the comments will just say Linux but should work with most *nixes

static void ThrowOnRelative(const char* envName, const char* envValue) {
    if (envValue[0] != '/') {
        char buffer[200];
        snprintf(buffer, sizeof(buffer), "Environment \"%s\" does not start with an '/'. XDG specifies that the value must be absolute. The current value is: \"%s\"", envName, envValue);
        throw std::runtime_error(buffer);
    }
}

static std::string GetHome() {
    std::string res;
    const char* tempRes = getenv("HOME");
    if (!tempRes) {
        throw std::runtime_error("The environment HOME is not defined");
    }
    res = tempRes;
    return res;
}

static std::string GetLinuxFolderDefault(const char* envName, const char* defaultRelativePath) {
    std::string res;
    const char* tempRes = getenv(envName);
    if (tempRes) {
        ThrowOnRelative(envName, tempRes);
        res = tempRes;
        return res;
    }
    res = GetHome() + "/" + defaultRelativePath;
    return res;
}

static void AppendExtraFoldersTokenizer(const char* envName, const char* envValue, std::vector<std::string>& folders) {
	char buffer[strlen(envValue)+1];
	strcpy(buffer, envValue);
    char *saveptr;
    const char* p = strtok_r (buffer, ":", &saveptr);
    while (p != NULL) {
        if (p[0] == '/') {
            folders.push_back(p);
        }
        else {
            //Unless the system is wrongly configured this should never happen... But of course some systems will be incorectly configured.
            //The XDG documentation indicates that the folder should be ignored but that the program should continue.
            std::cerr << "Skipping path \"" << p << "\" in \"" << envName << "\" because it does not start with a \"/\"" << std::endl;
        }
        p = strtok_r (NULL, ":", &saveptr);
    }
}

static void AppendExtraFolders(const char* envName, const char* defaultValue, std::vector<std::string>& folders) {
    const char* envValue = getenv(envName);
    if (!envValue) {
        envValue = defaultValue;
    }
    AppendExtraFoldersTokenizer(envName, envValue, folders);
}

#endif


namespace sago {

std::string GetDataHome() {
#if defined(_WIN32)
    return GetAppData();
#else
    return GetLinuxFolderDefault("XDG_DATA_HOME", ".local/share");
#endif
}

std::string GetConfigHome() {
#if defined(_WIN32)
    return GetAppData();
#else
    return GetLinuxFolderDefault("XDG_CONFIG_HOME", ".config");   
#endif
}

std::string GetCacheDir() {
#if defined(_WIN32)
    return GetAppDataLocal();
#else
    return GetLinuxFolderDefault("XDG_CONFIG_HOME", ".cache");
#endif
}

void AppendAdditionalDataDirectories(std::vector<std::string>& homes) {
#if defined(_WIN32)
    homes.push_back(GetAppDataCommon());
#else
    AppendExtraFolders("XDG_DATA_DIRS", "/usr/local/share/:/usr/share/", homes);
#endif
}

void AppendAdditionalConfigDirectories(std::vector<std::string>& homes) {
#if defined(_WIN32)
    homes.push_back(GetAppDataCommon());
#else
    AppendExtraFolders("XDG_CONFIG_DIRS", "/etc/xdg", homes);
#endif
}

#if defined(_WIN32)
#else
struct PlatformFolders::PlatformFoldersData {
	std::map<std::string, std::string> folders;
};

static void PlatformFoldersAddFromFile(const std::string& filename, std::map<std::string, std::string>& folders) {
	std::ifstream infile(filename.c_str());
	std::string line;
	while (std::getline(infile, line)) {
		if (line.length() == 0 || line.at(0) == '#') {
			continue;
		}
		std::size_t splitPos = line.find("=");
		std::string key = line.substr(0, splitPos);
		std::string value = line.substr(splitPos+2, line.length()-splitPos-3);
		folders[key] = value;
		//std::cout << key << " : " << value << std::endl;
	}
}

static void PlatformFoldersFillData(std::map<std::string, std::string>& folders) {
	folders["XDG_DOCUMENTS_DIR"] = "$HOME/Documents";
	folders["XDG_DESKTOP_DIR"] = "$HOME/Desktop";
	folders["XDG_DOWNLOAD_DIR"] = "$HOME/Downloads";
	folders["XDG_MUSIC_DIR"] = "$HOME/Music";
	folders["XDG_PICTURES_DIR"] = "$HOME/Pictures";
	folders["XDG_PUBLICSHARE_DIR"] = "$HOME/Public";
	folders["XDG_TEMPLATES_DIR"] = "$HOME/.Templates";
	folders["XDG_VIDEOS_DIR"] = "$HOME/Videos";
	PlatformFoldersAddFromFile( GetConfigHome()+"/user-dirs.dirs", folders);
	for (std::map<std::string, std::string>::iterator itr = folders.begin() ; itr != folders.end() ; itr ++ ) {
		std::string& value = itr->second;
		if (value.compare(0, 5, "$HOME") == 0) {
			value = GetHome() + value.substr(5, std::string::npos);
		}
	}
}
#endif

PlatformFolders::PlatformFolders() {
#if defined(_WIN32)
#else
	this->data = new PlatformFolders::PlatformFoldersData();
	try {
		PlatformFoldersFillData(data->folders);
	} catch (...) {
		delete this->data;
		throw;
	}
#endif
}

PlatformFolders::~PlatformFolders() {
#if defined(_WIN32)
#else
	delete this->data;
#endif
}

std::string PlatformFolders::GetDocumentsFolder() const {
#if defined(_WIN32)
	return GetWindowsFolder(CSIDL_PERSONAL, "Failed to find My Documents folder");
#else
	return data->folders["XDG_DOCUMENTS_DIR"];
#endif
}

std::string PlatformFolders::GetDesktopFolder() const {
#if defined(_WIN32)
	return GetWindowsFolder(CSIDL_DESKTOP, "Failed to find Desktop folder");
#else
	return data->folders["XDG_DESKTOP_DIR"];
#endif
}

std::string PlatformFolders::GetPicturesFolder() const {
#if defined(_WIN32)
	return GetWindowsFolder(CSIDL_MYPICTURES, "Failed to find My Pictures folder");
#else
	return data->folders["XDG_PICTURES_DIR"];
#endif
}

std::string PlatformFolders::GetDownloadFolder1() const {
#if defined(_WIN32)
	//Pre Vista. Files was downloaded to the desktop
	return GetWindowsFolder(CSIDL_DESKTOP, "Failed to find My Downloads (Desktop) folder");
#else
	return data->folders["XDG_DOWNLOAD_DIR"];
#endif
}

std::string PlatformFolders::GetMusicFolder() const {
#if defined(_WIN32)
	return GetWindowsFolder(CSIDL_MYMUSIC, "Failed to find My Music folder");
#else
	return data->folders["XDG_MUSIC_DIR"];
#endif
}

std::string PlatformFolders::GetVideoFolder() const {
#if defined(_WIN32)
	return GetWindowsFolder(CSIDL_MYVIDEO, "Failed to find My Video folder");
#else
	return data->folders["XDG_VIDEOS_DIR"];
#endif
}

std::string PlatformFolders::GetSaveGamesFolder1() const {
#if defined(_WIN32)
	//A dedicated Save Games folder was not introduced until Vista. For XP and older save games are most often saved in a normal folder named "My Games".
	//Data that should not be user accessible should be placed under GetDataHome() instead
	return GetWindowsFolder(CSIDL_PERSONAL, "Failed to find My Documents folder")+"/My Games";
#else
	return GetDataHome();
#endif
}



}  //namespace sago
