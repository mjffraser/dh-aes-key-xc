#include "debug.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace dh::debug {

void print_bytes_as_hex(const std::string &bytes) {
	std::stringstream ss;
	ss << "0x";
	for (size_t i = 0; i < bytes.size(); ++i) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)bytes[i];
	}

	std::cout << ss.str() << std::endl;
}

void print_bytes_as_hex(unsigned char* bytes, int len) {
	std::stringstream ss;
	ss << "0x";
	for (int i = 0; i < len; ++i) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[i];
	}
	std::cout << ss.str() << std::endl;
}

}

