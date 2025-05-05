#pragma once

#include "dh-aes-params.hpp"

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
 * -> Sends a message over a socket, encoding the length of the message at the
 *    start as 4-bytes.
 *
 * Takes:
 * -> socket:
 *    The connected socket to send over.
 * -> message:
 *    The message to send.
 * -> message_len:
 *    How long the message is.
 *
 * Returns:
 * -> On success:
 *    The total bytes sent. This should be message_len+4. 
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
ssize_t sendMessage(int                 socket,
                    char   const* const message,
                    size_t        const message_len);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * recvMessage
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Receives a message sent with the above function into a buffer.
 * 
 * Takes:
 * -> socket:
 *    The connected socket to receive the message from.
 * -> buffer:
 *    A buffer to receive data into.
 *
 * Returns:
 * -> On success:
 *    The length of the received message. This will be the message_len passed
 *    into sendMessage().
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
ssize_t recvMessage(int                socket,
                    std::vector<char>& buffer);

} //dh
