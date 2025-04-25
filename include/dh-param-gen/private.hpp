#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * privateA
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Picks a random exponent 1 < a < p. Stores a inside dh_v.
 *
 * Takes:
 * -> dh_v:
 *    The Diffie-Hellman value container with p initialized.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int privateA(DHParams& dh_v);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * publicA
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Computes and stores g^a mod p.
 *    This is the A value in most DH protocol specs. Stores A inside dh_v.
 *
 * Takes:
 * -> dh_v:
 *    The Diffie-Hellman value container with p, g, and a initialized.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int publicA(DHParams& dh_v);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * dhKey
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Computes and stores B^a mod p === g^ab mod p.
 *    This is the shared DH key. Stores dh_key inside of dh_v.
 *
 * Takes:
 * -> dh_v:
 *    The Diffie-Hellman value container with p, B, and a initialized.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int dhKey(DHParams& dh_v);

}
