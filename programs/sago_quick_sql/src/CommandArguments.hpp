#ifndef COMMANDARGUMENTS_HPP
#define COMMANDARGUMENTS_HPP

#include <string>
#include <vector>

struct CommandArguments {
	std::string connectstring = "mysql:database=testschema;user=testuser;password=password";
	std::string sqlstring = "";
	std::vector<std::string> bindArgs;
	bool doExec = false;
	int minCount = 0;
	int maxCount = -1;
	bool silent = false;
};

#endif /* COMMANDARGUMENTS_HPP */

