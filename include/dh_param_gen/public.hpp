#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * select_public_DH_params
 *
 * Returns the pair <p,g>, where p is a large prime, and
 * g is a primitive root of p.
 *
 * Depending on command line args a vetted pair <p,g> may 
 * be selected from a list.
 *
 * Pulls bits from params to select p. 
 *
 * Returns:
 * - {p,g} on sucess
 * - {-1,-1} on failure (will not occur until random p's are implemented.)
 */
std::pair<cpp_int, cpp_int> select_public_DH_params(Params& params);

}
