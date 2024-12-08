#pragma once

#include <string>
#include "dh_params.hpp"
namespace dh::debug {

void print_bytes_as_hex(cpp_int& val);
void print_bytes_as_hex(const std::string& bytes);
void print_bytes_as_hex(unsigned char* bytes, int len);

}
