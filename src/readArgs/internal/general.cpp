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
		if (arg == "--debug" || arg == "--db") {
			debug.value() = true;
		} 
	
		//vetted primes flag (DEFAULT - TRUE (use vetted primes))
		else if (arg == "--generatePrime" || arg == "--gp")  {
			useVettedPrimes = false;
		} 

		//log path (should be followed by path so: --log /home/.../log.txt)
		//relative paths are also allowed (so: --log log.txt)
		//we don't check the user input here, it's verified by the logger before it attempts to open it
		else if (arg == "--logPath" || "--log") {
			if ((i+1) < argc) {
				logPath.value() = argv[i+1];
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
