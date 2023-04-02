#include <iostream>
#include <boost/program_options.hpp>
#include "rhash.h"
#include <vector>

class rhash_wrapper {
	private:
		std::vector<unsigned> hash_ids; //ids used for 
		rhash_context* the_context = nullptr;
	public:
		rhash_wrapper() {
			hash_ids.push_back(RHASH_ED2K);
		}

		~rhash_wrapper() {
			if (the_context) {
				rhash_free(the_context);
			}
		}
};

#ifndef VERSIONNUMBER
#define VERSIONNUMBER "0.1.0"
#endif

static void hash_file(const std::string& filename, unsigned int hash_id = RHASH_SHA256) {
	unsigned char buffer[1024];
	char output[1024];
	int result = rhash_file(hash_id, filename.c_str(), buffer);
	if (result) {
		std::cout << "Failed with errorcode " << errno << "\n";
		exit(1);
	}
	rhash_print_bytes(output, buffer, rhash_get_digest_size(hash_id),RHPR_HEX);
	std::cout << "HEX:" << output << "\n";
	rhash_print_bytes(output, buffer, rhash_get_digest_size(hash_id),RHPR_BASE32);
	std::cout << "BASE32:" << output << "\n";
}

struct file_hashes {
	std::string sha256;
	std::string md5;
	std::string filename;
	std::string sha3_512;
	std::string ed2k_hash;
	std::string aich;
	size_t filesize = 0;
};

std::string get_hash(const char* filename, unsigned int hash_id, unsigned int format) {
	unsigned char buffer[1024];
	char output[1024];
	int result = rhash_file(hash_id, filename, buffer);
	if (result) {
		std::cout << "Failed with errorcode " << errno << "\n";
		exit(1);
	}
	rhash_print_bytes(output, buffer, rhash_get_digest_size(hash_id),format);
	return std::string(output);
}

void calculate_hashes(file_hashes& output, const char* filename) {
	output = {};
	std::vector<unsigned> hash_ids = {RHASH_SHA256, RHASH_MD5, RHASH_SHA3_512, RHASH_ED2K, RHASH_AICH};
	unsigned flags = 0;
	for (unsigned flag : hash_ids) {
		flags |= flag;
	}
	rhash_context* context = rhash_init(flags);
	if (!context) {
		std::cout << "Failed to init context. Error no: " << errno << "\n";
		exit(1);
	}
	output.sha256 = get_hash(filename, RHASH_SHA256, RHPR_HEX);
	output.md5 = get_hash(filename, RHASH_MD5, RHPR_HEX);
	output.filename = filename;
	output.sha3_512 = get_hash(filename, RHASH_SHA3_512, RHPR_HEX);
	output.ed2k_hash = get_hash(filename, RHASH_ED2K, RHPR_HEX);
	output.aich = get_hash(filename, RHASH_AICH, RHPR_BASE32);
	output.filesize = 0;
	rhash_free(context);
}

int main(int argc, const char* argv[]) {
	boost::program_options::options_description desc("Options");
	desc.add_options()
	("version", "Print version information and quit")
	("help,h", "Print basic usage information to stdout and quit")
	("filename", boost::program_options::value<std::string>(), "A file to process")
	;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}
	if (vm.count("version")) {
		std::cout << "community-archiver " << VERSIONNUMBER << "\n";
		return 0;
	}
	rhash_library_init();
	if (vm.count("filename")) {
		std::string filename = vm["filename"].as<std::string>();
		//hash_file(filename);
		//hash_file(filename, RHASH_ED2K);
		//hash_file(filename, RHASH_AICH);
		file_hashes fh;
		calculate_hashes(fh, filename.c_str());
		std::cout << "sha256: " << fh.sha256 << "\n";
		std::cout << "md5: " << fh.md5 << "\n";
		std::cout << "filename: " << fh.filename << "\n";
	}
	else {
		std::cout << "This program demonstrates boost::program_options. Try \"" << argv[0] << " --help\" or \"" << argv[0] << " --somestring hello\"\n";
	}
	return 0;
}
