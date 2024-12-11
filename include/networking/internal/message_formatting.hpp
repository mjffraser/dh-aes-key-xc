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
 * Takes the hex string from above and converts it back
 * to unsigned char's.
 */
void htos(std::string& ciphertext_h, unsigned char* ciphertext, size_t len);


using Message = std::pair<char*, size_t>;

/*
 * format_message
 *
 * Creates a message of the form:
 *	one||two||...||n-1||n
 *
 * where one, two, ..., n-1, n, are the n messages in 
 * strings.
 *
 * Returns:
 * - the message
 */
std::string format_message( std::initializer_list<Message> strings);

/*
 * parse_message
 *
 * Strips the above formatting.
 *
 * Places the messages one, two, ..., n-1, n, into a vector and
 * returns it where they're the n messages put through format_message().
 *
 * Returns:
 * - the vector with the induvidual messages.
 */
std::vector<std::string> parse_message(const std::string& formatted);

}
