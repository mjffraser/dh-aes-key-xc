#include "dhParams.hpp"
#include "readArgs/internal/general.hpp"

#include <filesystem>
#include <regex>

namespace dh {

//unix paths allow any character except the NUL char. 
static const std::regex unixPattern{R"(^[^\0]*$)"};

//matches end of path ending in alphanumeric char.
//slightly restricts naming space, but makes sure a name was specified
//Would match both /log and /.log but no /. or /log.
static const std::regex filePattern(R"(^.*[a-zA-Z0-9]$)");

bool validateLogPath(const std::string& path) {
	if (path.empty()) {
		std::cout << "[WARN] No file name supplied. Using default log name and location." << std::endl;
		return false;
	}

	//convert the path to absolute if it's relative
	std::string absPath = std::filesystem::absolute(path);

	//try path
	bool result = std::regex_match(absPath, unixPattern);
	if (!result)
		std::cout << "[WARN] Your supplied log file path doesn't appear to be a valid unix path? Using default log name and location." << std::endl;

	result = std::regex_match(absPath, filePattern);
	if (!result)
		std::cout << "[WARN] Can't parse a valid file name for the log. Using default log name and location." << std::endl;
	return result;
}

void parseGeneralFields(int argc, char* argv[]) {
	std::optional<bool>					debug           = std::nullopt;
	std::optional<bool>					useVettedPrimes = std::nullopt;
	std::optional<std::string>	logPath         = std::nullopt;

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);
		
		//debug flag (DEFAULT - FALSE)
		if (arg == "--debug" || arg == "-d") {
			debug = true;
		} 
	
		//vetted primes flag (DEFAULT - TRUE (use vetted primes))
		else if (arg == "--generatePrimes")  {
			useVettedPrimes = false;
		} 

		//log path (should be followed by path so: --log /home/.../log.txt)
		//relative paths are also allowed (so: --log log.txt)
		else if (arg == "--logPath" || arg == "-l") {
			if ((i+1) < argc) {
				std::string path(argv[i+1]);
				if (validateLogPath(path)) 
					logPath = path;
				++i; //extra increment to jump over path
			} else {
				std::cout << "[WARN] No log path supplied. USAGE: --logPath [path]" << std::endl;
			}
		}
	}

	DHParams& config = DHParams::get();	
	config.setGeneralFields(logPath, debug, useVettedPrimes);
}	

}
