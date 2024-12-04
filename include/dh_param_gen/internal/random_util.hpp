#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * rand_between
 *
 * returns a random cpp_int from the range (lower, upper).
 *
 * draws bytes from a ND source from the OS.
 */
cpp_int rand_between(cpp_int& lower, cpp_int& upper);

/*
 * rand_prime (NOT IMPLEMENTED)
 *
 * returns a random safe prime that is bits in length.
 *
 * draws bytes from a ND source from the OS.
 */
cpp_int rand_prime  (unsigned int bits);

}
