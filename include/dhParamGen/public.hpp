#pragma once

#include "num.hpp"

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
std::pair<Num, Num> selectPublicDHParams();

}
