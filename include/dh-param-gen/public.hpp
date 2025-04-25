#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * selectPublicDHParams
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Selects a prime p and generator g based on the bits value in the
 *    ConfigParams.
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
