#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * create_client
 *
 * Creates client socket.
 *
 * Returns:
 * - socket descriptor on success.
 * - a negative result on error.
 */
int create_client(Params& params);

/* 
 * client_teardown
 *
 * Cleans up the client socket. Should be called either
 * after finishing the communication, or after any error
 * before exiting.
 */
void client_teardown(int socket);

/* connect_to_server
 *
 * Attempts to connect to the server at IP:Port
 *
 * Returns:
 * - 0 on success.
 * - a negative result on error.
 */
int connect_to_server(int socket, Params& params);

/*
 * recv_dh_pub
 *
 * Contacts the server and gets the public p&g values
 * used for the key establishment. Puts p&g into 
 * params.
 *
 * Returns:
 * - 0 on success.
 * - a negative result on error.
 */
int recv_dh_pub(int socket, Params& params);

/*
 * send_A
 *
 * Sends g^a to the server.
 *
 * Returns:
 * - 0 on success.
 * - a negative result on error.
 */
int send_server_A(int client, Params& params);

/*
 * recv_B
 *
 * Receives B from the server into params.
 *
 *
 * Returns:
 * - 0 on success.
 * - a negative result on error.
 */
int recv_server_B(int client, Params& params);

/*
 * send_encrypted_message
 *
 * Sends the AES encrypted message to the server.
 *
 * Expects:
 * - client socket
 * - params with dh_key set.
 * - message
 * - message length
 *
 * Returns:
 * - 0 on success.
 * - a negative result on error.
 */
int send_encrypted_message(int client,
													 Params& params,
													 const char* message, 
													 size_t message_len);
}


