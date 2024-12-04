#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * aes_keygen
 *
 * Performs a computationally hard hashing algorithm to 
 * generate a 256-bit AES key from the agreed DH key, as 
 * well as a 128-bit initialization vector.
 *
 * returns the pair of raw bytes in std::string's in the form:
 * <KEY, IV>
 */
std::pair<std::string, std::string> aes_keygen(cpp_int& dh_shared_key);

}
