#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include "CommandArguments.hpp"

extern const std::string SAGO_PREFIX;

// Resolves a "sago:<alias>" connection string in-place by reading the first
// line of <config_home>/sago_quick_sql/connections/<alias>.txt.
// Non-alias connection strings are left untouched.
// Throws std::runtime_error on missing/empty alias or file.
void ResolveConnectionAlias(std::string& connectstring);

// Returns the alias name from a "sago:<alias>" string, or empty if not aliased.
std::string ExtractAlias(const std::string& connectstring);

// Appends a single log line for the request to
// <cache_dir>/sago_quick_sql/logs/<alias>.log (or "generic.log" if alias is empty).
// Best-effort: silently returns if the file cannot be opened.
void LogRequest(const std::string& alias, const CommandArguments& args);

#endif /* CONNECTION_HPP */
