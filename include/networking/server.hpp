#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * create_server
 *
 * Creates server socket and returns the socket descriptor
 */
int create_server(Params& params);

/*
 * server_teardown
 *
 * Cleans up the server socket. Should be called either
 * after finishing the communication, or after any error
 * before exiting.
 */
void server_teardown(int server, int client = -1);

/*
 * accept_client
 *
 * Returns 
 * - socket descriptor for an incoming client connection.
 * - a negative result on failure. Terminate server socket 
 *   in this case.
 */
int accept_client(int server, Params& params);

/* 
 * send_p_g
 *
 * Sends p & g to the client. 
 *
 * Returns: 
 * - bytes sent (message len)
 * - a negative result on failure, terminate both sockets
 *   in this case.
 */
int send_p_g(int client, Params& params);

/*
 * recv_A
 *
 * Recieves B from client. 
 *
 * Returns:
 * - the cpp_int B on success,
 * - a negative result on failure, terminate both sockets
 *   in this case.
 */
int recv_client_B(int client, Params& params);

/*
 * send_A
 *
 * Sends A to client.
 *
 * Returns:
 * - a negative result on failture, terminate both sockets
 *   in this case.
 */
int send_client_A(int client, Params& params);

/*
 * recv_encrypted_message
 *
 * Receives the AES encrypted message and tag from the client,
 * decrypts it, and writes to the provided plaintext buffer. 
 *
 * This function expects that the client just received B, and is
 * calculating the AES key, which is slow. An extra timeout delay
 * is inserted while waiting to receive the message.
 *
 * Expects:
 * - buffer: a variable size buffer to write the message to.
 *
 * Returns a negative result on failure.
 */
int recv_encrypted_message(int client,
													 Params& params,
													 std::vector<char>& buffer);

}
