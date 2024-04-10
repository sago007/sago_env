#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <cppdb/frontend.h>
#include <cppdb/backend.h>
#include <chrono>
#include <iomanip>

namespace po = boost::program_options;

const std::string VERSION_NUMBER = "0.0.1";

struct CommandArguments {
	std::string connectstring = "mysql:database=testschema;user=testuser;password=password";
	std::string csv_file;
	std::string table;
	std::vector<std::string> dest_columns;
	std::vector<std::string> csv_columns;
	std::vector<std::string> key_fields;
	std::vector<std::string> parse_iso_dates;
};

template <class Container> 
const bool contains(const Container& container, const typename Container::value_type& element) 
{
	return std::find(container.begin(), container.end(), element) != container.end();
}

std::chrono::system_clock::time_point parseISO8601(const std::string& timestamp) {
    std::tm tm{};
    std::istringstream ss(timestamp);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse timestamp");
    }

    // Read the microseconds and timezone offset separately
    std::string microseconds;
    ss >> microseconds;

    // Extract timezone offset
    int tz_hour = 0, tz_min = 0;
    if (!microseconds.empty() && microseconds.back() == 'Z') {
        // UTC timezone
    } else if (microseconds.size() > 2 && microseconds[microseconds.size() - 3] == ':') {
        std::istringstream tz_ss(microseconds.substr(0, microseconds.size() - 3));
        char colon;
        tz_ss >> tz_hour >> colon >> tz_min;
    } else {
        throw std::runtime_error("Invalid timezone format");
    }

    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    auto offset = std::chrono::hours(tz_hour) + std::chrono::minutes(tz_min);
    if (microseconds != "Z") {
        if (microseconds[0] == '-') {
            tp -= offset;
        } else {
            tp += offset;
        }
    }

    return tp;
}

std::vector<std::string> split_str(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = 0;
	while ((end = str.find(delimiter, start)) != std::string::npos) {
		result.push_back(str.substr(start, end - start));
		start = end + delimiter.size();
	}
	result.push_back(str.substr(start));
	return result;
}

CommandArguments cmdargs;



//Environment to ensure that it is spelled consistently
const char* const SAGO_CONNECTION_STRING = "SAGO_CONNECTION_STRING";

void upsert_csv_to_mysql(const std::string& csv_file, const std::string& table, const std::vector<std::string>& dest_columns,
                         const std::vector<std::string>& csv_columns, const std::vector<std::string>& key_fields,
                         const std::vector<std::string>& parse_iso_dates) {
	try {
		cppdb::session sql(cmdargs.connectstring);
		
		std::ifstream file(csv_file);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open CSV file.");
		}

		std::string line;
		int lines_inserted = 0;
		int lines_updated = 0;
		while (std::getline(file, line)) {
			//std::string value;
			std::vector<std::string> values = split_str(line, ",");
			for (size_t i = 0; i < values.size(); ++i) {
				if (contains(parse_iso_dates, dest_columns[i])) {
					// Parse ISO dates here if needed
					values[i] = std::to_string(parseISO8601(values[i]).time_since_epoch().count()/1000/1000/1000);
				}
			}
			std::vector<std::string> update_values = values;
			std::string insert_query = "INSERT INTO " + table + " (";
			for (size_t i = 0; i < dest_columns.size(); ++i) {
				if (i > 0) {
					insert_query += ", ";
				}
				insert_query += dest_columns[i];
			}
			insert_query += ") VALUES (";
			std::string update_query = "UPDATE " + table + " SET ";
			std::string where_clause = " WHERE ";
			bool first_column = true;

			for (size_t i = 0; i < dest_columns.size(); ++i) {
				if (i > 0) {
					insert_query += ", ";
					update_query += ", ";
				}
				if (contains(parse_iso_dates, dest_columns[i])) {
					// Parse ISO dates here if needed
					insert_query += "FROM_UNIXTIME(?)";
					update_query += dest_columns[i] + "=FROM_UNIXTIME(?)";
				}
				else {
					insert_query += "?";
					update_query += dest_columns[i] + "=?";
				}
			}

			for (const auto& key_field : key_fields) {
				if (!first_column) {
					where_clause += " AND ";
				}
				where_clause += key_field + "=?";
				first_column = false;
			}

			insert_query += ")";
			update_query += where_clause;

			std::cout << "update_query: " << update_query << std::endl;
			std::cout << "insert_query: " << insert_query << std::endl;

			cppdb::statement ins = sql << insert_query;
			cppdb::statement upd = sql << update_query;

			for (size_t i = 0; i < values.size(); ++i) {
				ins.bind(values[i]);
				upd.bind(values[i]);
			}

			try {
				ins.exec();
				lines_inserted++;
			} catch (const std::exception& e) {
				std::cout << "Insert failed, trying update. Error: " << e.what() << std::endl;
				upd.exec();
				lines_updated++;
			}
		}

		sql.commit();
		std::cout << "Inserted " << lines_inserted << " lines and updated " << lines_updated << " lines." << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

int main(int argc, char** argv) {
	po::options_description desc("Allowed options");
	desc.add_options()
		("version", "Print version number and quit")
		("help,h", "Print basic usage information to stdout and quit")
		("connect-string", po::value<std::string>(), "String used for connecting. Like \"odbc:DSN=mydb\"")
		("csv-file", po::value<std::string>(), "CSV file to import")
		("table", po::value<std::string>(), "Table to import to")
		("dest-columns", po::value<std::string>(), "Columns to import to")
		("csv-columns", po::value<std::string>(), "Columns in CSV file")
		("key-fields", po::value<std::string>(), "Key fields for upsert")
		("parse-iso-dates", po::value<std::string>(), "Fields to parse from ISO date format")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		if (getenv(SAGO_CONNECTION_STRING)) {
			std::cout << "The environment " << SAGO_CONNECTION_STRING << " is currently set\n";
		}
		return 0;
	}

	if (vm.count("version")) {
		std::cout << VERSION_NUMBER << std::endl;
		return 0;
	}

	bool connectstring_set = false;
	const char* connectstring_env = getenv(SAGO_CONNECTION_STRING);
	if (connectstring_env) {
		cmdargs.connectstring = connectstring_env;
		connectstring_set = true;
	}
	if (vm.count("connect-string")) {
		cmdargs.connectstring = vm["connect-string"].as<std::string>();
		connectstring_set = true;
	}
	if (!connectstring_set) {
		std::cerr << "connect-string not set, using default: \"" << cmdargs.connectstring << "\"\n";
	}
	if (!vm.count("csv-file")) {
		std::cerr << "csv-file not set\n";
		return 1;
	}
	if (!vm.count("table")) {
		std::cerr << "table not set\n";
		return 1;
	}
	if (!vm.count("dest-columns")) {
		std::cerr << "dest-columns not set\n";
		return 1;
	}
	if (!vm.count("csv-columns")) {
		std::cerr << "csv-columns not set\n";
		return 1;
	}
	if (!vm.count("key-fields")) {
		std::cerr << "key-fields not set\n";
		return 1;
	}
	std::string reading_arg = "None";
	try{
		reading_arg = "csv-file";
		cmdargs.csv_file = vm["csv-file"].as<std::string>();
		reading_arg = "table";
		cmdargs.table = vm["table"].as<std::string>();
		reading_arg = "dest-columns";
		cmdargs.dest_columns = split_str(vm["dest-columns"].as<std::string>(),",");
		reading_arg = "csv-columns";
		cmdargs.csv_columns = split_str(vm["csv-columns"].as<std::string>(),",");
		reading_arg = "key-fields";
		cmdargs.key_fields = split_str(vm["key-fields"].as<std::string>(),",");
		if (vm.count("parse-iso-dates")) {
			reading_arg = "parse-iso-dates";
			cmdargs.parse_iso_dates = split_str(vm["parse-iso-dates"].as<std::string>(),",");
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << " while reading \"" << reading_arg << "\"\n";
		return 1;
	}


	upsert_csv_to_mysql(cmdargs.csv_file, 
						cmdargs.table, 
						cmdargs.dest_columns, 
						cmdargs.csv_columns, 
						cmdargs.key_fields,
						cmdargs.parse_iso_dates);

	return 0;
}

