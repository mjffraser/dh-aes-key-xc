#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * aes_keygen
 *
 * Performs a computationally hard hashing algorithm to 
 * generate a 256-bit AES key from the agreed DH key, as 
 * well as a 96-bit initialization vector.
 *
 * This IV should never be used to encrypt a second message 
 * under the same key. A new nonce should be generated, ideally
 * guarranteed to be different than the one returned here.
 *
 * This can be done via incrementing up/down.
 *
 * returns the pair of raw bytes in std::string's in the form:
 * <KEY, IV>
 */
std::pair<std::string, std::string> aes_keygen(cpp_int& dh_shared_key, Params& params);


}
