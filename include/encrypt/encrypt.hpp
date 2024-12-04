#pragma once

#include <string>

namespace dh {

/*
 * encrypt_message
 *
 * Takes a plaintext and converts it to a ciphertext via AES-256.
 *
 * The 256-bit key should be obtained from keygen.hpp
 */
std::string encrypt_message(std::string& message, std::string& aes_key);

}
