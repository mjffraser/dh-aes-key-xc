#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * dhAesKXC
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Carries out main Diffie-Hellman key exchange, AES keygen, and message
 *    sending between client and server. The client sends the first message, and
 *    messages are exhanged back and forth between server and client from that
 *    point. 
 *
 *    This function expects that all setup regarding the Logger and read-args is
 *    complete BEFORE calling this.
 *
 * Takes:
 * -> params:
 *    The ConfigParams object, fully initialized.
 *
 * Returns:
 * -> On success (or at any point of not being able to continue post the first
 *    message, ie either side ends the connection):
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int dhAesKXC(ConfigParams const& params);

} //dh
