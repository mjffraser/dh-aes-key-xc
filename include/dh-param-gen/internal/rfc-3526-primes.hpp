#pragma once

#include <cstdint>
#include <string>

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * vettedP
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Returns a prime p from RFC3526 that is bits in size. If bits is not one of
 *    the valid sizes {1536, 2048, 3072, 4096, 6144, 8192}, an error is
 *    returned.
 *
 * Takes:
 * -> bits:
 *    How large p should be.
 *
 * Returns:
 * -> On success:
 *    p, as a hex-string.
 * -> On failure:
 *    The empty string.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
std::string vettedP(uint16_t bits);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * vettedG
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Returns a generator for a prime p. As all RFC3526 primes have g=2 as a
 *    valid choice, this essentially just returns 2. It's written this way to be
 *    easier to expand in the future if desired.
 *
 * Returns:
 * -> On success:
 *    2
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
uint8_t vettedG();

} //dh
