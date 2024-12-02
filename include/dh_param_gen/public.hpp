#pragma once

#include "dh_params.hpp"

//Generation/Selection for public DH values done by server.

namespace dh {

/*
 * selectPublicDHParams
 *
 * returns the pair <p,g>, where p is a large prime, and
 * g is a primitive root of p.
 *
 * depending on command line args a vetted pair <p,g> may 
 * be selected from a list.
 */
std::pair<cpp_int, cpp_int> select_public_DH_params();

}
