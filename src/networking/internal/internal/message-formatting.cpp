#include "networking/internal/internal/message-formatting.hpp"

namespace dh {

std::string itoh(cpp_int const& big_int) {
	std::ostringstream oss;
	oss << std::hex << std::setw(2) << std::setfill('0') << big_int;
	return oss.str();
}

cpp_int	htoi(std::string const& h_str) {
	return cpp_int("0x" + h_str);
}

std::string stoh(unsigned char const* const data, 
                 size_t               const len) {
	std::ostringstream oss;
	oss << std::hex << std::setfill('0');
	for (size_t i = 0; i < len; ++i) 
		oss << std::setw(2) << (int)data[i];
	return oss.str();
}

void htos(std::string    const& data_hex,
          unsigned char* const  data_dest,
          size_t         const  len) {
	for (size_t i = 0; i < len; ++i) {
		std::string byte = data_hex.substr(i*2, 2);
		data_dest[i] = (unsigned char)std::stoul(byte, nullptr, 16);
	}	
}


std::string formatMessage(std::initializer_list<Message> const strings) {
	std::string formatted;
	size_t limit = strings.size(), index = 0;
	for (auto& [msg, len] : strings) {
		std::string s(msg, msg+len);
		formatted.append(msg, msg+len);
		if (index != limit-1) 
			formatted += "||";

		index++;
	}
	return formatted;
}

std::vector<std::string> parseMessage(std::string const& formatted) {
	std::string formatted_copy = formatted;
	std::vector<std::string> split_msg; 
	size_t pos;
	while ((pos = formatted_copy.find("||")) != std::string::npos) {
		split_msg.push_back( formatted_copy.substr(0, pos) );
		formatted_copy.erase(0, pos + 2); //2 for length of ||
	}

	split_msg.push_back(formatted_copy); 

	return split_msg;
}

} //dh
