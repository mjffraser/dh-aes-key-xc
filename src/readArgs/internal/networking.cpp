#include "dhParams.hpp"
#include <regex>
#include <string>
#include <unordered_set>
#include "readArgs/internal/networking.hpp"

namespace dh {

//sourced from: https://stackoverflow.com/questions/5284147/validating-ipv4-addresses-with-regexp
static const std::regex ipPattern{R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)"};

bool validateIP(const std::string& ip) {
	if (ip.empty()) {
		std::cout << "[WARN] No IP address supplied. Using localhost." << std::endl;
		return false;
	}

	bool result = std::regex_match(ip, ipPattern);
	if (!result)
		std::cout << "[WARN] IP Address supplied doesn't appear to be valid IPv4 address. Using localhost." << std::endl;
	return result;
}

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
			serverFlag = true;
		} else if (arg == "--client" || arg == "-c") {
			serverFlag = false;
		} 

		//bits
		else if (arg == "--pBits" || arg == "-b") {
			if ((i+1) < argc) {
				unsigned int bitsVal = (unsigned int) std::stoul(argv[i+1]);
				
				//supporting 1536, 2048 (DEFAULT), 3072, 4096, 6144, 8192
				std::unordered_set<unsigned int> validBits = {1536, 2048, 3072, 4096, 6144, 8192};
        if (validBits.count(bitsVal)) {
					bits = bitsVal;
        } else {
					std::cout << "[WARN] Supplied bit amount for prime p is not one of {1536, 2048, 3072, 4096, 6144, 8192}." << std::endl;
          std::cout << "Supplied bit value: " << bitsVal << std::endl;
					std::cout << "Default (2048) will be used." << std::endl;
        }	
				
				++i;
			}
		}

		//ip
		else if (arg == "--ip" || "-i") {
			if ((i+1) < argc) {
				std::string addr(argv[i+1]);
				if (validateIP(addr))
					ip = addr;
				++i;
			}
		}
			
		//port number
		else if (arg == "--port" || arg == "-p") {
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
				port = portNo;
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
