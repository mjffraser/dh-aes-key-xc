#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * create_client
 *
 * Creates client socket and returns the socket descriptor
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
 * - 0 on success,
 * - a negative result on error.
 */
int connect_to_server(int socket, Params& params);

/*
 * recv_dh_pub
 *
 * Contacts the server and gets the public p&g values
 * used for the key establishment.
 *
 * Returns a negative value on failure.
 */
int recv_dh_pub(int socket, Params& params);

/*
 * send_A
 *
 * Sends g^a to the server.
 *
 * Returns a negative value on failure.
 */
int send_server_A(int client, Params& params);

/*
 * recv_B
 *
 * Receives g^b from the server. 
 *
 * This function expects that the server just received A, 
 * and is calculating the AES key, which is slow. An extra
 * timeout delay is inserted while waiting to receive B.
 *
 * Returns a negative value on failure.
 */
int recv_server_B(int client, Params& params);

/*
 * send_encrypted_message
 *
 * Sends the AES encrypted message to the server.
 *
 * Expects:
 * - message: the encrypted data, as a hex string
 * - messsage_len
 * - tag: a hex string representing the 16-byte tag.
 *		- should be 32 chars long
 * - iv: a hex string representing the 16-byte nonce.
 *		- should be 32 chars long
 *
 * Returns a negative value on failure.
 */
int send_encrypted_message(int client,
													 Params& params,
													 const char* message, 
													 size_t message_len);
}


