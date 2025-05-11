#pragma once

#include "dh-aes-params.hpp"
#include <optional>

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * createSocket
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Opens a TCP socket with the parameters in the ConfigParams object. These
 *    were either verified when taken from command line, or defaults are used.
 *
 * Takes:
 * -> params:
 *    The config object with ip, port, and the server flag for binding.
 *
 * Returns:
 * -> On success:
 *    The socket fd.
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int createSocket(ConfigParams const& params);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * closeSocket
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Calls close() on socket provided.
 *
 * Takes:
 * -> socket:
 *    The socket to close.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
void closeSocket(int socket);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * initConnection:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Initiates a connection with the other side.
 *    -> If a server:
 *       Starts listening, waits for a client to connect, returns socket opened
 *       by accept().
 *    -> If a client:
 *       Connects to the server address inside the ConfigParams object.
 *
 * Takes:
 * -> socket:
 *    -> If server: the bound socket to start listening with.
 *    -> If client: the socket to connect with.
 * -> params:
 *    The ConfigParams object to draw IP, port, and server flag from.
 *
 * Returns:
 * -> On success:
 *    -> If server:
 *       Client socket fd.
 *    -> If client:
 *       0
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int initConnection(int socket, ConfigParams const& params);

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
