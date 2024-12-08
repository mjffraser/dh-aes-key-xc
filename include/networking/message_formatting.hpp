#pragma once

#include "dh_params.hpp"

namespace dh {

std::string itoh(cpp_int& i);
cpp_int	htoi(const std::string& h);

using Message = std::pair<unsigned char*, size_t>;
std::string format_message( std::initializer_list<Message> strings);
std::vector<std::string> parse_message(const std::string& formatted);

}
