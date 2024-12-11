#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * create_server
 *
 * Creates server socket.
 *
 * Returns:
 * - socket descriptor on success.
 * - a negative value on error.
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
 * Sends p||g to the client. 
 *
 * Returns: 
 * - 0 on success 
 * - a negative result on failure, terminate both sockets
 *   in this case.
 */
int send_p_g(int client, Params& params);

/*
 * recv_A
 *
 * Receives B from client into params. 
 *
 * Returns:
 * - 0 on success
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
 * - 0 on success
 * - a negative result on failure, terminate both sockets
 *   in this case.
 */
int send_client_A(int client, Params& params);

/*
 * recv_encrypted_message
 *
 * Receives the AES encrypted message and tag from the client,
 * decrypts it, and writes to the provided plaintext buffer. 
 *
 * Expects:
 * - client socket
 * - params containing dh_key
 * - buffer: a variable size buffer to write the message to.
 *
 * Returns:
 * - 0 on success
 * - a negative result on failure, terminate both sockets
 *   in this case.
 */
int recv_encrypted_message(int client,
													 Params& params,
													 std::vector<char>& buffer);

}
