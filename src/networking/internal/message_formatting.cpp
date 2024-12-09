#include "networking/internal/message_formatting.hpp"

namespace dh {

std::string itoh(cpp_int &i) {
	std::ostringstream oss;
	oss << std::hex << std::setw(2) << std::setfill('0') << i;
	return oss.str();
}

cpp_int htoi(const std::string& h) {
	return cpp_int("0x"+h);
}

std::string format_message(std::initializer_list<Message> strings) {
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

std::vector<std::string> parse_message(const std::string& formatted) {
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

}
