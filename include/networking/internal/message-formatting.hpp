#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * itoh
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Returns the hexadecimal representation of the provided multiprecision
 *    cpp_int.
 *
 * Takes:
 * -> big_int:
 *    The big int to convert.
 *
 * Returns:
 * -> On success:
 *    The hexadecimal representation as a string.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
std::string itoh(cpp_int const& big_int);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * htoi
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> The reverse of the above function.
 *
 * Takes:
 * -> h_str:
 *    The hex string to convert.
 *
 * Returns:
 * -> On success:
 *    The cpp_int of the hexadecimal number.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
cpp_int	htoi(std::string const& h_str);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * stoh
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Takes a string of unsigned char's and converts it to a hex string.
 *
 * Takes:
 * -> data:
 *    The data to convert.
 * -> len:
 *    The length of the data.
 * 
 * Returns:
 * -> On success:
 *    The bytes formatted as hex.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
std::string stoh(unsigned char const* const data, 
                 size_t               const len);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * htos
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Converts a hex string produced by the above function back to unsigned
 *    char's.
 *
 * Takes:
 * -> data_hex:
 *    The hex string.
 * -> data_dest:
 *    The buffer to store the converted data inside of. Should be len in length.
 * -> len:
 *    The len of the buffer. Should be data_hex.length()/2 in length, as stoh
 *    produces strings that have setw(2). 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void htos(std::string    const& data_hex,
          unsigned char* const  data_dest,
          size_t         const  len);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * formatMessage
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> For every the n strings provided in the initializer list, denoted as
 *    n[0], n[1], ..., n[n-1], builds a string "n[0]||n[1]||...||n[n-1]".
 *
 * Takes:
 * -> strings:
 *    The list of strings to append with a delimiter.
 *
 * Returns:
 * -> On success:
 *    The constructed string.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
using Message = std::pair<char*, size_t>;
std::string formatMessage(std::initializer_list<Message> const strings);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parseMessage
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Parses a list of messages constructed into a single string by the above
 *    function and returns the list of strings.
 *
 * Takes:
 * -> formatted:
 *    The string produced by formatMessage().
 *
 * Returns:
 * -> On success:
 *    A vector of the strings that were formatted.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
std::vector<std::string> parseMessage(std::string const& formatted);

}
