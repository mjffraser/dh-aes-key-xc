#include "dhParams.hpp"
#include <string>
#include <unordered_set>
#include "readArgs/internal/networking.hpp"

namespace dh {

int parseNetworkingFields(int argc, char* argv[]) {
	//note that setNetworkFields wants a bool value. serverFlag being std::optional is to check it's actually set
	std::optional<bool>					serverFlag = std::nullopt; 
	std::optional<unsigned int> bits       = std::nullopt; 
	std::optional<std::string>	ip         = std::nullopt;
	std::optional<unsigned int> port       = std::nullopt;

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);

		//server/client flag
		if (arg == "--server" || arg == "-s") {
			serverFlag.value() = true;
		} else if (arg == "--client" || arg == "-c") {
			serverFlag.value() = false;
		} 

		//bits
		else if (arg == "--pBits" || "-b") {
			if ((i+1) < argc) {
				unsigned int bitsVal = (unsigned int) std::stoul(argv[i+1]);
				
				//supporting 1024, 2048 (DEFAULT), 4096
				std::unordered_set<unsigned int> validBits = {1024, 2048, 4096};
        if (validBits.count(bitsVal)) {
					bits.value() = bitsVal;
        } else {
					std::cout << "[WARN] Supplied bit amount for prime p is not one of {1024, 2048, 4096}." << std::endl;
          std::cout << "Supplied bit value: " << bitsVal << std::endl;
					std::cout << "Default (2048) will be used." << std::endl;
        }	

				++i;
			}
		}

		//ip
		//TODO: add regex to validate
		else if (arg == "--ip" || "-i") {
			if ((i+1) < argc) {
				std::string addr(argv[i+1]);
				//validate
				ip.value() = addr;
				++i;
			}
		}
			
		//port number
		else if (arg == "--port" || "-p") {
			if ((i+1) < argc) {
				unsigned int portNo = (unsigned int) std::stoul(argv[i+1]);

				if (portNo < 1024) {
					std::cout << "[WARN] Supplied port number is probably in use by the system. A random high port will be selected." << std::endl;
					std::cout << "Recommended port range: 49152-65535." << std::endl;
				} else if (portNo > 65535) {
					std::cout << "[WARN] Supplied port number is too high! A random high port will be selected." << std::endl;
					std::cout << "Recommended port range: 49152-65535." << std::endl;
				}
				
				++i;
				port.value() = portNo;
			}
		}
	}

	//check that either server or client was specified.
	if (!serverFlag) {
		std::cerr << "[ERR] Specified as neither client nor server. What am I?" << std::endl;
		std::cerr << "USAGE: -c or --client for client, -s or --server for server." << std::endl;
		return 1;
	}

	DHParams& config = DHParams::get();
	config.setNetworkFields(serverFlag.value(), bits, ip, port);
	return 0;
}

}
