#include "platform_folders.h"
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <stdio.h>
#include <cstdlib>

#ifdef WIN32

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
#ifdef WIN32
    return GetAppData();
#else
    return GetLinuxFolderDefault("XDG_DATA_HOME", ".local/share");
#endif
}

std::string GetConfigHome() {
#ifdef WIN32
    return GetAppData();
#else
    return GetLinuxFolderDefault("XDG_CONFIG_HOME", ".config");   
#endif
}

std::string GetCacheDir() {
#ifdef WIN32
    return GetAppDataLocal();
#else
    return GetLinuxFolderDefault("XDG_CONFIG_HOME", ".cache");
#endif
}

void AppendAdditionalDataDirectories(std::vector<std::string>& homes) {
#ifdef WIN32
    homes.push_back(GetAppDataCommon());
#else
    AppendExtraFolders("XDG_DATA_DIRS", "/usr/local/share/:/usr/share/", homes);
#endif
}

void AppendAdditionalConfigDirectories(std::vector<std::string>& homes) {
#ifdef WIN32
    homes.push_back(GetAppDataCommon());
#else
    AppendExtraFolders("XDG_CONFIG_DIRS", "/etc/xdg", homes);
#endif
}


}  //namespace sago