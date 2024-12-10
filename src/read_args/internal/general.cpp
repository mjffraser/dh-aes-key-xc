#include "dh_params.hpp"
#include "read_args/internal/general.hpp"

#include <filesystem>
#include <regex>

namespace dh {

//unix paths allow any character except the NUL char. 
static const std::regex unix_pattern{R"(^[^\0]*$)"};

//matches end of path ending in alphanumeric char.
//slightly restricts naming space, but makes sure a name was specified
//Would match both /log and /.log but no /. or /log.
static const std::regex file_pattern(R"(^.*[a-zA-Z0-9]$)");

bool validate_log_path(const std::string& path) {
	if (path.empty()) {
		std::cout << "[WARN] No file name supplied. Using default log name and location." << std::endl;
		return false;
	}

	//convert the path to absolute if it's relative
	std::string abs_path = std::filesystem::absolute(path);

	//try path
	bool result = std::regex_match(abs_path, unix_pattern);
	if (!result)
		std::cout << "[WARN] Your supplied log file path doesn't appear to be a valid unix path? Using default log name and location." << std::endl;

	result = std::regex_match(abs_path, file_pattern);
	if (!result)
		std::cout << "[WARN] Can't parse a valid file name for the log. Using default log name and location." << std::endl;
	return result;
}

void parse_general_fields(int argc, char* argv[], Params& params) {
	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);
		
		//debug flag (DEFAULT - FALSE)
		if (arg == "--debug") {
			params.debug = true;	
		} 

		else if (arg == "--quiet") {
			params.quiet = true;
		}

		else if (arg == "--verbose") {
			params.verbose = true;
		}
	
		//log path (should be followed by path so: --log /home/.../log.txt)
		//relative paths are also allowed (so: --log log.txt)
		else if (arg == "--log") {
			if ((i+1) < argc) {
				std::string path(argv[i+1]);
				if (validate_log_path(path)) 
					params.log_path = path;
				++i; //extra increment to jump over path
			} else {
				std::cout << "[WARN] No log path supplied. USAGE: --logPath [path]" << std::endl;
			}
		} 

		//combined flags like: -sdq for server, debug, quiet
		else if (arg.find("--") == std::string::npos &&
						 arg.find("-")  != std::string::npos) {
			if (arg.find("q") != std::string::npos)
				params.quiet = true;
			if (arg.find("d") != std::string::npos)
				params.debug = true;
			if (arg.find("v") != std::string::npos)
				params.verbose = true;
		}
	}
}	

}
