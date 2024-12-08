#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * create_client
 *
 * Creates client socket and returns the socket descriptor
 */
int create_client();

/* 
 * client_teardown
 *
 * Cleans up the client socket. Should be called either
 * after finishing the communication, or after any error
 * before exiting.
 */
void client_teardown(int socket);

/*
 * recv_dh_pub
 *
 * Contacts the server and gets the public p&g values
 * used for the key establishment.
 *
 * Returns a negative value on failure.
 */
int recv_dh_pub(cpp_int& p, cpp_int& g);

/*
 * send_A
 *
 * Sends g^a to the server.
 *
 * Returns a negative value on failure.
 */
int send_A(cpp_int& A);

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
int recv_B(cpp_int& B);

/*
 * send_encrypted_message
 *
 * Sends the AES encrypted message to the server.
 *
 * Expects:
 * - message: the encrypted data, as bytes.
 * - messsage_len: the number of bytes in message.
 * - tag: the 16-byte tag for authentication on the other side.
 * - iv: the 16-byte iv used for encryption. 
 *
 * Returns a negative value on failure.
 */
int send_encrypted_message(unsigned char* message, 
													 int message_len, 
													 unsigned char* tag, 
													 unsigned char* iv);

}

/* 1) Create socket.
 * 2) Get p&g. 
 *		2.1) Calculate a, A.
 * 3) Send A 
 * 4) Receive B
 *		4.1) Calculate DH_key
 *		4.2) Compute AES key (SLOW)
 * 5) Send encrypted message
 */
