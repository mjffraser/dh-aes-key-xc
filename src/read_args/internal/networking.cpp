#include "dh_params.hpp"
#include <optional>
#include <regex>
#include <string>
#include <unordered_set>
#include "read_args/internal/networking.hpp"

namespace dh {

//sourced from: https://stackoverflow.com/questions/5284147/validating-ipv4-addresses-with-regexp
static const std::regex ip_pattern{R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)"};

bool validate_IP(const std::string& ip) {
	if (ip.empty()) {
		std::cout << "[WARN] No IP address supplied. Using localhost." << std::endl;
		return false;
	}

	bool result = std::regex_match(ip, ip_pattern);
	if (!result)
		std::cout << "[WARN] IP Address supplied doesn't appear to be valid IPv4 address. Using localhost." << std::endl;
	return result;
}

int parse_networking_fields(int argc, char* argv[], Params& params) {
	//note that set_network_fields wants a bool value. server_flag being std::optional is to check it's actually set
	std::optional<bool>	server_flag = std::nullopt;

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i]);

		//server/client flag
		if (arg == "--server") {
			server_flag = true;
		} else if (arg == "--client") {
			server_flag = false;
		} 

		//bits
		else if (arg == "--bits") {
			if ((i+1) < argc) {
				unsigned int bits_val = (unsigned int) std::stoul(argv[i+1]);
				
				//supporting 1536, 2048 (DEFAULT), 3072, 4096, 6144, 8192
				std::unordered_set<unsigned int> valid_bits = {1536, 2048, 3072, 4096, 6144, 8192};
        if (valid_bits.count(bits_val)) {
					params.bits = bits_val;
        } else {
					std::cout << "[WARN] Supplied bit amount for prime p is not one of {1536, 2048, 3072, 4096, 6144, 8192}." << std::endl;
          std::cout << "Supplied bit value: " << bits_val << std::endl;
					std::cout << "Default (2048) will be used." << std::endl;
        }	
				
				++i;
			}
		}

		//ip
		else if (arg == "--ip") {
			if ((i+1) < argc) {
				std::string addr(argv[i+1]);
				if (validate_IP(addr))
					params.ip_addr = addr;
				++i;
			}
		}
			
		//port number
		else if (arg == "--port") {
			if ((i+1) < argc) {
				unsigned int port_no = (unsigned int) std::stoul(argv[i+1]);

				if (port_no < 1024) {
					std::cout << "[WARN] Supplied port number is probably in use by the system, using default of 65000." << std::endl;
					std::cout << "Recommended port range: 49152-65535." << std::endl;
				} else if (port_no > 65535) {
					std::cout << "[WARN] Supplied port number is too high! Using default of 65000." << std::endl;
					std::cout << "Recommended port range: 49152-65535." << std::endl;
				} else {
					params.port = port_no;
				}
				
				++i;
			}
		} 

		else if (arg.find("--") == std::string::npos &&
						 arg.find("-")  != std::string::npos) {
			if (arg.find("s") != std::string::npos)
				server_flag = true;
			if (arg.find("c") != std::string::npos)
				server_flag = false;
			if (arg.find("s") != std::string::npos &&
					arg.find("c") != std::string::npos) {
				std::cerr << "[ERR] Specified as both client and server." << std::endl;
				return -1;
			}
		}
	}

	//check that either server or client was specified.
	if (!server_flag) {
		std::cerr << "[ERR] Specified as neither client nor server. What am I?" << std::endl;
		std::cerr << "USAGE: -c or --client for client, -s or --server for server." << std::endl;
		return -1;
	} else {
		params.server = server_flag.value();
	}

	return 0;
}

}
