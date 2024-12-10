#pragma once

#include <memory>
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
 * random_bytes
 *
 * draws count bytes from a ND source from the OS and returns them.
 */
std::shared_ptr<unsigned char> random_bytes(int count);

}
