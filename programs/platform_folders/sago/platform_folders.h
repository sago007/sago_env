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

}  //namespace sago

#endif	/* PLATFORM_FOLDERS_H */

