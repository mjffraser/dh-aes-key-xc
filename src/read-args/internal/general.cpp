#include "dh-aes-params.hpp"
#include "logger.hpp"
#include "read-args/internal/general.hpp"

#include <filesystem>
#include <regex>

namespace dh {

//unix paths allow any character except the NUL char. 
std::regex static const unix_pattern{R"(^[^\0]*$)"};

//matches end of path ending in alphanumeric char.
//slightly restricts naming space, but makes sure a name was specified
//would match both /log and /.log but no /. or /log.
std::regex static const file_pattern(R"(^.*[a-zA-Z0-9]$)");

bool validateLogPath(std::string const& path, Logger& log) {
	if (path.empty()) {
    log.warn("No file name supplied. Using default log name and location.");
		return false;
	}

	//convert the path to absolute if it's relative
	std::string abs_path = std::filesystem::absolute(path);

	//try path
	bool result = std::regex_match(abs_path, unix_pattern);
	if (!result)
    log.warn("Your supplied log file path doesn't appear to be a valid unix path? Using default log name and location.");

	result = std::regex_match(abs_path, file_pattern);
	if (!result)
    log.warn("Can't parse a valid file name for the log. Using default log name and location.");
	return result;
}


std::string static const help_msg = "USAGE: ./dh-key-xc <args>\n"
																		"FOR AVAILABLE <args> SEE docs/usage.md\n"
																		"MIN SERVER USAGE: ./dh-key-xc -s\n"
																		"MIN CLIENT USAGE: ./dh-key-xc -c";

int parseGeneralFields(int argc, char* argv[], ConfigParams& config) {
  Logger& log = Logger::get();

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);
		
		//debug flag (DEFAULT - FALSE)
		if (arg == "--debug") {
			config.debug = true;	
		} 

		else if (arg == "--quiet") {
			config.quiet = true;
		}

		else if (arg == "--verbose") {
			config.verbose = true;
		}
	
		//log path (should be followed by path so: --log /home/.../log.txt)
		//relative paths are also allowed (so: --log log.txt)
		else if (arg == "--log") {
			if ((i+1) < argc) {
				std::string path(argv[i+1]);
				if (validateLogPath(path, log)) 
					config.log_path = path;
				++i; //extra increment to jump over path
			} else {
        log.warn("No log path supplied. USAGE: --log <path>");
			}
		} 

		else if (arg == "--help") {
			std::cout << help_msg << '\n';
			return EXIT_FAILURE;
		}

		//combined flags like: -sdq for server, debug, quiet
		else if (arg.find("--") == std::string::npos &&
						 arg.find("-")  != std::string::npos) {
			if (arg.find("q") != std::string::npos) config.quiet   = true;
			if (arg.find("d") != std::string::npos) config.debug   = true;
			if (arg.find("v") != std::string::npos) config.verbose = true;
			if (arg.find("h") != std::string::npos) {
				std::cout << help_msg << '\n';
				return EXIT_FAILURE;
			}
		}
	}

	return EXIT_SUCCESS;
}	

} //dh

