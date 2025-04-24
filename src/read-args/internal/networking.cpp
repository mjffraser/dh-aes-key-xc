#include "read-args/internal/networking.hpp"
#include "logger.hpp"

#include <optional>
#include <regex>
#include <unordered_set>

namespace dh {

//sourced from: https://stackoverflow.com/questions/5284147/validating-ipv4-addresses-with-regexp
std::regex static const ip_pattern{R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)"};

bool validateIP(const std::string& ip, Logger& log) {
	if (ip.empty()) {
		log.warn("No IP address supplied. Using localhost.");
		return false;
	}

	bool result = std::regex_match(ip, ip_pattern);
	if (!result)
		log.warn("IP Address supplied doesn't appear to be valid IPv4 address. Using localhost.");
	return result;
}

int parseNetworkingFields(int argc, char* argv[], ConfigParams& config) {
  Logger& log = Logger::get();

	//note that set_network_fields wants a bool value.
  //server_flag being std::optional is to check it's actually set
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
				uint16_t bits_val = (uint16_t)std::stoul(argv[i+1]);
				
				//supporting 1536, 2048 (DEFAULT), 3072, 4096, 6144, 8192
				std::unordered_set<unsigned int> valid_bits = {1536, 2048, 3072, 4096, 6144, 8192};
        if (valid_bits.count(bits_val)) {
					config.bits = bits_val;
        } else {
          std::stringstream ss;
          ss << "Supplied bit amount for prime p is not one of {1536, 2048, 3072, 4096, 6144, 8192}." << '\n';
          ss << "Supplied bit value: " << bits_val << '\n';
					ss << "Default (2048) will be used.";
          log.warn(ss.str());
        }	
				++i; //jump over the arg for --bits
			}
		}

		//ip
		else if (arg == "--ip") {
			if ((i+1) < argc) {
				std::string addr(argv[i+1]);
				if (validateIP(addr, log))
					config.ip_addr = addr;
				++i; //jump over the arg for --ip
			}
		}
			
		//port number
		else if (arg == "--port") {
			if ((i+1) < argc) {
				uint16_t port_no = (uint16_t)std::stoul(argv[i+1]);

				if (port_no < 1024) {
          std::stringstream ss;
          ss << "Supplied port number is probably in use by the system, using default of 65000." << '\n';
					ss << "Recommended port range: 49152-65535.";
          log.warn(ss.str());
				} else if (port_no > 65535) {
          std::stringstream ss;
					ss << "Supplied port number is too high! Using default of 65000." << '\n';
					ss << "Recommended port range: 49152-65535.";
          log.warn(ss.str());
				} else {
					config.port = port_no;
				}
				++i; //jump over arg for --port
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
				log.err("Specified as both client and server.");
				return EXIT_FAILURE;
			}
		}
	}

	//check that either server or client was specified.
	if (!server_flag) {
    std::stringstream ss;
    ss << "Specified as neither client nor server. What am I?" << '\n';
		ss << "USAGE: -c or --client for client, -s or --server for server.";
    log.err(ss.str());
		return EXIT_FAILURE;
	} else {
		config.server = server_flag.value();
	}

	return EXIT_SUCCESS;
} 

} //dh
