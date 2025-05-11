#pragma once

#include <sys/types.h>
#include <vector>

namespace dh {

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

}
