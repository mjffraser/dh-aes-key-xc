#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * itoh
 *
 * Converts a cpp_int to it's hex string.
 *
 * Returns the string of the bytes in i 
 * interpreted as hex.
 */
std::string itoh(cpp_int& i);

/*
 * htoi
 *
 * Puts the value the hex string h represents
 * into a cpp_int.
 *
 * Returns the value as a cpp_int.
 */
cpp_int	htoi(const std::string& h);

/*
 * stoh
 *
 * Takes a string of unsigned char's and converts 
 * it to hex. 
 */
std::string stoh(unsigned char* ciphertext, size_t len);

/*
 * htos
 *
 * Takes the string from above and converts it back
 * to unsigned char's.
 */
void htos(std::string& ciphertext_h, unsigned char* ciphertext, size_t len);


using Message = std::pair<char*, size_t>;
std::string format_message( std::initializer_list<Message> strings);
std::vector<std::string> parse_message(const std::string& formatted);

}
