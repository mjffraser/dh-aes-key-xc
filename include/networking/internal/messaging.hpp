#pragma once

#include "dh-aes-params.hpp"

#include <optional>

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sendMessage
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Sends a message over the provided socket. If an AESParams object is also
 *    provided, encrypt the message with the key and nonce, then send it.
 *    Encrypted messages are converted to hex strings for the transmission to
 *    avoid ambiguity.
 *
 * Takes:
 * -> socket:
 *    The socket to use.
 * -> message:
 *    The message to send. If encryption is NOT performed (no aes_v), this is
 *    sent as-is, and should already be formatted for the other side to
 *    understand.
 * -> aes_v:
 *    A container with the values needed for encryption. If present, the message
 *    is encrypted.
 *
 * Returns:
 * -> On success:
 *    EXIT_FAILURE
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int sendMessage(int                             socket,
                std::string              const& message,
                std::optional<AESParams> const& aes_v=std::nullopt);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * recvMessage
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Receieves a message over the provided socket. If an AESParams object is
 *    also provided, assumes the message is encrypted, and will attempt to
 *    decrypt it with the values inside aes_v.
 *
 * Takes:
 * -> socket:
 *    The socket to use.
 * -> message:
 *    Where to write the received (and possibly decrypted) message to.
 * -> aes_v:
 *    A container with the values needed for decryption. If present, the message
 *    decrypted if possible with the sent tag.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int recvMessage(int                             socket,
                std::string&                    message,
                std::optional<AESParams> const& aes_v=std::nullopt);

} //dh
