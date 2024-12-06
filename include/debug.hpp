#pragma once

#include <string>
namespace dh::debug {

void print_bytes_as_hex(const std::string& bytes);
void print_bytes_as_hex(unsigned char* bytes, int len);

}
