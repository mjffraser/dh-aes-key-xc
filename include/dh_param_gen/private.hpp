#pragma once

#include "dh_params.hpp" 

//Generation for private DH values done by server and client.

namespace dh {

/*
 * generate_a 
 *
 * Picks a random exponent a between 1 and p-1.
 *
 * Expects p to be initialized already inside 
 * params.
 *
 * Returns:
 * - a on success
 * - -1 on failure
 */
cpp_int generate_a(Params& params);

/*
 * generate_A
 *
 * Computes g^a mod p.
 *
 * Expects p, g, and a are all initialized
 * inside params.
 *
 * Returns:
 * - A on sucess
 * - -1 on failure
 */
cpp_int generate_A(Params& params);

/*
 * generate_key
 *
 * Computes B^a mod p.
 *
 * Expects p, a, and B are all initalized
 * inside params.
 *
 * Returns:
 * - dh_key on sucess
 * - -1 on failure
 */
cpp_int generate_key(Params& params);

}
