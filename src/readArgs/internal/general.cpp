#include "dhParams.hpp"
#include "readArgs/internal/general.hpp"

namespace dh {

void parseGeneralFields(int argc, char* argv[]) {
	std::optional<bool>					debug           = std::nullopt;
	std::optional<bool>					useVettedPrimes = std::nullopt;
	std::optional<std::string>	logPath         = std::nullopt;

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);
		
		//debug flag (DEFAULT - FALSE)
		if (arg == "--debug" || arg == "-d") {
			debug.value() = true;
		} 
	
		//vetted primes flag (DEFAULT - TRUE (use vetted primes))
		else if (arg == "--generatePrimes")  {
			useVettedPrimes = false;
		} 

		//log path (should be followed by path so: --log /home/.../log.txt)
		//relative paths are also allowed (so: --log log.txt)
		//TODO: add path regex
		else if (arg == "--logPath" || "-l") {
			if ((i+1) < argc) {
				std::string path(argv[i+1]);
				//validate
				logPath.value() = path;
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
