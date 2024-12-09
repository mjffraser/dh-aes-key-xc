#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * encrypt
 *
 * Takes a plaintext and converts it to a ciphertext via AES-256
 * in Galois Counter Mode.
 *
 * The 256-bit key, and 128-bit IV should be obtained via keygen.hpp 
 *
 * returns: length of ciphertext
 */
int encrypt(unsigned char* plaintext,
						int						 plaintext_len,
						unsigned char* key,
						unsigned char* iv,
						unsigned char* ciphertext,
						unsigned char* tag,
						Params&				 params);

/*
 * decrypt
 *
 * Takes a ciphertext and converts it to a plaintext via AES-256
 * in Galois Counter Mode.
 *
 * The 256-bit key, and 128-bit IV should be obtained via keygen.hpp
 *
 * returns:
 * - length of plaintext on success,
 * - a negative result on failure to authenticate the ciphertext
 */

int decrypt(unsigned char* ciphertext,
						int						 ciphertext_len,
						unsigned char* tag,
						unsigned char* key,
						unsigned char* iv,
						unsigned char* plaintext,
						Params&				 params);
	

}
