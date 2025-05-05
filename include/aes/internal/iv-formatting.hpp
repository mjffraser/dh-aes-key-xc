#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * formIV
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Copies the multiprecision cpp_int into the destination buffer, only
 *    returning success if the cpp_int provided is <= to the size of the buffer
 *    provided as iv_len.
 *
 * Takes:
 * -> dest:
 *    The buffer to copy the bytes into. Must match iv_len's size or a
 *    seg-fault is almost certain.
 * -> current_iv:
 *    The 12-byte IV to convert to bytes and copy.
 * -> iv_len:
 *    The length of the dest.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int formIV(unsigned char* dest, cpp_int const& current_iv, size_t iv_len=12);

int formCppInt(cpp_int& dest, unsigned char* src, size_t src_len=12);
 
}
