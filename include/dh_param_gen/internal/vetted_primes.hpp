#pragma once

#include <string>

namespace dh {

/*
 * get_vetted_p
 *
 * Get a prime p that is bits in size.
 *
 * If a vetted prime of bits size isn't available,
 * use 2048 as the default.
 *
 * Returns p as a hexstring.
 */
std::string get_vetted_p(unsigned int bits);

/* 
 * get_vetted_g
 *
 * Get a primitive root g for p.
 *
 * g=2 for all vetted primes p. 
 *
 * Returns g as int.
 */
int get_vetted_g();

}

