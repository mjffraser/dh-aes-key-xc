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

void parse_general_fields(int argc, char* argv[]) {
	std::optional<bool>					debug             = std::nullopt;
	std::optional<bool>					use_vetted_primes = std::nullopt;
	std::optional<std::string>	log_path          = std::nullopt;

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);
		
		//debug flag (DEFAULT - FALSE)
		if (arg == "--debug" || arg == "-d") {
			debug = true;
		} 
	
		//vetted primes flag (DEFAULT - TRUE (use vetted primes))
		else if (arg == "--generatePrimes")  {
			use_vetted_primes = false;
		} 

		//log path (should be followed by path so: --log /home/.../log.txt)
		//relative paths are also allowed (so: --log log.txt)
		else if (arg == "--logPath" || arg == "-l") {
			if ((i+1) < argc) {
				std::string path(argv[i+1]);
				if (validate_log_path(path)) 
					log_path = path;
				++i; //extra increment to jump over path
			} else {
				std::cout << "[WARN] No log path supplied. USAGE: --logPath [path]" << std::endl;
			}
		}
	}

	DH_Params& config = DH_Params::get();	
	config.set_general_fields(log_path, debug, use_vetted_primes);
}	

}
