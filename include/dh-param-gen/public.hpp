#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * selectPublicDHParams
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Selects a prime p and generator g based on the bits value in the
 *    ConfigParams. If an error is returned, the prime p that was selected does
 *    not match the size of config.bits
 *
 *    In this case, bits is probably a bad size, and should be adjusted to p's
 *    size.
 *
 * Takes:
 * -> config:
 *    The ConfigParams container that has the number of bits p should be.
 * -> dh_v:
 *    The container to store p&g inside of.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int selectPublicDHParams(ConfigParams const& config,
                         DHParams&           dh_v);

} //dh
