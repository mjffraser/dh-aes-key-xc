#pragma once

#include <cstddef>
#include <sys/types.h>
#include <vector>

namespace dh {

/*
 * create_socket
 *
 * Returns a socket. If it's a socket for a server, also binds it to
 * the supplied port number. IPv4 only.
 */
int create_socket();

/*
 * close_socket
 *
 * Does teardown on socket. 
 *
 * This should ALWAYS be called on any exit of the program if a socket
 * has been created. No function will attempt to call this, it should
 * only be called via the main process creating and managing the socket.
 */
void close_socket(int socket);


/*
 * init_connection
 *
 * Server socket:
 * - sets socket to listen for client,
 *   accepts client connection,
 *   returns client socket descriptor
 *
 * Client socket:
 * - attempts to connect to server at stored IP:Port,
 *	 binds client socket to server,
 *   returns 0
 *
 * A negative result is returned on error for either process.
 */
int init_connection(int socket);

/*
 * send_message
 *
 * Sends a message to the client/server.
 *
 * Returns bytes sent (len) on success, and a negative result on error.
 */
ssize_t send_message(int socket, const char* message, size_t len);

/*
 * recv_message
 *
 * Receives a message of unknown size into the end of the buffer. Appends
 * the data rather than overwriting, since it may take several calls of 
 * this function to receive the entire message. Will timeout after the 
 * provided number of seconds. 
 *
 * Returns:
 * - bytes received on sucess
 * - 0 on successful tranmission of entire message. This signals the entire
 *   message is in the buffer.
 * - a negative result on error.
 */
ssize_t recv_message(int socket, std::vector<unsigned char>& buffer, int timeout);

}