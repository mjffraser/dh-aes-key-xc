#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/* 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aesKeygen
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Runs the generated dh_key in dh_v through the scrypt KDF. We generate a
 *    352-bit hash of dh_key, with the first 256-bits corresponding to the AES
 *    key, and the remaining 96 are used as the IV.
 *
 *    On success, the generated AES key and IV are stored in aes_v.
 *
 * Takes:
 * -> dh_v:
 *    The Diffie-Hellman value container with dh_key populated.
 * -> aes_v:
 *    The container to store the resulting aes_key and aes_iv.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int aesKeygen(DHParams   const& dh_v,
              AESParams&        aes_v);

} //dh
