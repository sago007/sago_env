#ifndef PLATFORM_FOLDERS_H
#define	PLATFORM_FOLDERS_H

#include <vector>
#include <string>

/**
 * The namespace I use for common function. Nothing special about it. 
 */
namespace sago {

/**
 * Retrives the base folder for storring data files. 
 * You must add the program name yourself like this:
 * @code{.cpp}
 * string data_home = GetDataHome()+"/My Program Name/";
 * @endcode
 * On Windows this defaults to %APPDATA% (Roaming profile)
 * On Linux this defaults to ~/.local/share but can be configured
 * @return The base folder for storring program data.
 */
std::string GetDataHome();
/**
 * Retrives the base folder for storring config files. 
 * You must add the program name yourself like this:
 * @code{.cpp}
 * string data_home = GetConfigHome()+"/My Program Name/";
 * @endcode
 * On Windows this defaults to %APPDATA% (Roaming profile)
 * On Linux this defaults to ~/.config but can be configured
 * @return The base folder for storring config data.
 */
std::string GetConfigHome();
/**
 * Retrives the base folder for storring cache files. 
 * You must add the program name yourself like this:
 * @code{.cpp}
 * string data_home = GetCacheDir()+"/My Program Name/";
 * @endcode
 * On Windows this defaults to %APPDATALOCAL%
 * On Linux this defaults to ~/.cache but can be configured
 * @return The base folder for storring data that do not need to be backed up.
 */
std::string GetCacheDir();
/**
 * This will append extra folders that your program should be looking for data files in.
 * This does not normally include the path returned by GetDataHome().
 * If you want all the folders you should do something like:
 * @code{.cpp}
 * vector<string> folders;
 * folders.push_back(GetDataHome());
 * AppendAdditionalDataDirectories(folders);
 * for (string s& : folders) {
 *     s+="/My Program Name/";
 * }
 * @endcode
 * You must apply "/My Program Name/" to all the strings.
 * The string at the lowest index has the highest priority. 
 * @param homes A vector that extra folders will be appended to.
 */
void AppendAdditionalDataDirectories(std::vector<std::string>& homes);
/**
 * This will append extra folders that your program should be looking for config files in.
 * This does not normally include the path returned by GetConfigHome().
 * If you want all the folders you should do something like:
 * @code{.cpp}
 * vector<string> folders;
 * folders.push_back(GetConfigHome());
 * AppendAdditionalConfigDirectories(folders);
 * for (string s& : folders) {
 *     s+="/My Program Name/";
 * }
 * @endcode
 * You must apply "/My Program Name/" to all the strings.
 * The string at the lowest index has the highest priority. 
 * @param homes A vector that extra folders will be appended to.
 */
void AppendAdditionalConfigDirectories(std::vector<std::string>& homes);

/**
 * This class contains methods for finding the system depended special folders.
 * For Windows these folders are either by convention or given by CSIDL.
 * For Linux XDG convention is used.
 * The Linux version has very little error checking and assumes that the config is correct
 */
class PlatformFolders {
public:
	PlatformFolders();
	~PlatformFolders();
	/**
	 * The folder that represents the desktop.
	 * Normally you should try not to use this folder.
     * @return Absolute path to the user's desktop
     */
	std::string GetDesktopFolder() const;
	/**
	 * The folder to store user documents to
     * @return Absolute path to the "Documents" folder
     */
	std::string GetDocumentsFolder() const;
	/**
	 * The folder for storring the user's pictures.
     * @return Absolute path to the "Picture" folder
     */
	std::string GetPicturesFolder() const;
	/**
	 * The folder where files are downloaded.
	 * @note Windows: This version is XP compatible and returns the Desktop. Vista and later has a dedicated folder.
     * @return Absolute path to the folder where files are downloaded to.
     */
	std::string GetDownloadFolder1() const;
	/**
	 * The folder where music is stored
     * @return Absolute path to the music folder
     */
	std::string GetMusicFolder() const;
	/**
	 * The folder where video is stored
     * @return Absolute path to the video folder
     */
	std::string GetVideoFolder() const;
	/**
	 * The base folder for storring saved games.
	 * You must add the program name to it like this:
	 * @code{.cpp}
	 * PlatformFolders pf;
	 * string saved_games_folder = pf.GetSaveGamesFolder1()+"/My Program Name/";
	 * @endcode
	 * @note Windows: This is an XP compatible version and returns the path to "My Games" in Documents. Vista and later has an official folder.
	 * @note Linux: XDF does not define a folder for saved games. This will just return the same as GetDataHome()
     * @return The folder base folder for storring save games.
     */
	std::string GetSaveGamesFolder1() const;
private:
	PlatformFolders(const PlatformFolders&);  
	PlatformFolders& operator=(const PlatformFolders&);
	struct PlatformFoldersData;
	mutable PlatformFoldersData *data;
};

}  //namespace sago

#endif	/* PLATFORM_FOLDERS_H */

