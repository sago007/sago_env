#include <iostream>
#include <boost/program_options.hpp>
#include "onemap.hpp"
#include <regex>
#include <iterator>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

const char* const arenaText = "//Tier 1\n"
"{\n"
"map		\"oa_rpg3dm2\"\n"
"longname	\"Trial By Error\"\n"
"bots		\"ayumi jenna\"\n"
"fraglimit	\"15\"\n"
"type		\"single ffa lms tourney \"\n"
"}\n"
"{\n"
"map		\"oa_minia\"\n"
"longname	\"Kit's Mini Arena\"\n"
"bots		\"tony penguin\"\n"
"fraglimit	\"15\"\n"
"type		\"single ffa lms tourney elimination dd\"\n"
"}";

using namespace std;

namespace {
	void PrintMap(const OneMap& m, ostream &out_p = std::cout) {
		out_p << "{" << endl;
		out_p << "map\t\"" << m.name << "\"" <<endl;
		out_p << "longname\t\"" << m.longname << "\"" <<endl;
		if (m.special.length()) {
			out_p << "special\t\"" << m.special << "\"" <<endl;
		}
		if (m.bots.size()) {
			out_p << "bots\t\"";
			for (const string& i : m.bots) {
				out_p << i << " ";
			}
			out_p << "\"" << endl;
		}
		if (m.fraglimit > 0) {
			out_p << "fraglimit\t\"" << m.fraglimit << "\"" <<endl;
		}
		if (m.timelimit > 0) {
			out_p << "timelimit\t\"" << m.timelimit << "\"" <<endl;
		}
		if (m.gametype.size()) {
			out_p << "gametype\t\"";
			for (const string& i : m.gametype) {
				out_p << i << " ";
			}
			out_p << "\"" << endl;
		}
		out_p << "}" << endl;
	}
	
	std::string CompressWhitespace(std::string input) {
		std::regex whitespaceRegEx("[\\s]+");
		boost::trim(input);
		return std::regex_replace(input, whitespaceRegEx, " ");
	}
	
	void ParseTokensAndAppendMaps(const vector<string>& tokens, vector<OneMap>& maps) {
		bool expectOpenening = true;
		OneMap m;
		for( size_t i = 0; i < tokens.size(); i++) {
			if (expectOpenening) {
				if (tokens.at(i) == "{") {
					expectOpenening = false;
					m = {};
					continue;
				}
				cerr << "Failed to find {" << endl;
				exit(2);
			}
			if (tokens.at(i) == "map") {
				i++;
				m.name = tokens.at(i);
				continue;
			}
			if (tokens.at(i) == "longname") {
				i++;
				m.longname = tokens.at(i);
				continue;
			}
			if (tokens.at(i) == "bots") {
				i++;
				string tok = CompressWhitespace(tokens.at(i));
				boost::split(m.bots, tok , boost::is_any_of(" "), boost::token_compress_on);
				continue;
			}
			if (tokens.at(i) == "fraglimit") {
				i++;
				m.fraglimit = atoi(tokens.at(i).c_str());
				continue;
			}
			if (tokens.at(i) == "timelimit") {
				i++;
				m.timelimit = atoi(tokens.at(i).c_str());
				continue;
			}
			if (tokens.at(i) == "type") {
				i++;
				string tok = CompressWhitespace(tokens.at(i));
				boost::split(m.gametype, tok, boost::is_any_of(" "), boost::token_compress_on);
				continue;
			}
			if (tokens.at(i) == "}") {
				maps.push_back(m);
				continue;
			}
		}
	}
	
	void ParseAndAppendMaps(const std::string& text, vector<OneMap>& maps ) {
		std::regex commentRegEx("//[^\n]*\n", std::regex_constants::basic);
		std::string textNoComments = std::regex_replace(text, commentRegEx, "");
		//cout << textNoComments << endl;
		textNoComments = CompressWhitespace(textNoComments);
		boost::escaped_list_separator<char> els("\\"," \t\n","\"");
		boost::tokenizer<boost::escaped_list_separator<char> > tok(textNoComments, els);
		vector<string> tokens;
		tokens.assign(tok.begin(),tok.end());
		/*for (const string& item : tokens) {
			if (item.empty()) {
				continue;
			}
			cout << item << endl;
		}*/
		ParseTokensAndAppendMaps(tokens, maps);
	}
}  //namespace

int main(int argc, const char* const argv[])
{
	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print basic usage information to stdout and quits")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}
	vector<OneMap> maps;
	ParseAndAppendMaps(arenaText, maps);
	for( const OneMap m : maps) {
		PrintMap(m);
	}
	return 0;
}
