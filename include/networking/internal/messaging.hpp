#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Ensures values are in aes_v, encrypts the provided message, and sends it
 *    over the socket.
 *
 *    DOES NOT INCREMENT THE NONCE.
 *
 * Takes:
 * -> socket:
 *    The socket to use.
 * -> message:
 *    The message to send.
 * -> aes_v:
 *    The AESParams object to use for encryption.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int sendEncryptedMessage(int                socket, 
                         std::string const& message, 
                         AESParams   const& aes_v);
/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Ensures values are in aes_v, receives an encrypted message, decrypts if
 *    possible, and writes the result into the std::string message.
 *
 *    DOES NOT INCREMENT THE NONCE.
 *
 * Takes:
 * -> socket:
 *    The socket to use.
 * -> message:
 *    The message to receive.
 * -> aes_v:
 *    The AESParams object to use for decryption.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int recvEncryptedMessage(int                 socket,
                         std::string&        message,
                         AESParams    const& aes_v);

} //dh
