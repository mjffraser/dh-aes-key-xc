#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * create_server
 *
 * Creates server socket and returns the socket descriptor
 */
int create_server();

/*
 * accept_client
 *
 * Returns the socket descriptor for an incoming client connection.
 */
int accept_client();

/* 
 * establish_DH_key
 *
 * Engages in a series of messages with the client to establish
 * a shared key for the AES encryption.
 *
 * Returns: 
 * - a negative result on failure, the client socket should be terminated in this case.
 * - the shared key as a cpp_int otherwise.
 */
cpp_int establish_DH_key();

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
 * - tag: a 16-byte container to store the tag for the encrypted message.
 * - iv: a 16-byte contaner to store the iv used for the encryption.
 *
 * Returns a negative result on failure.
 */
int recv_encrypted_message(std::vector<unsigned char>& buffer, 
													 unsigned char* tag, 
													 unsigned char* iv);

}

/* 0) Calculate p, g, b, B
 * 1) Create socket. 
 * 2) Receive client, send client p&g.
 * 3) Receive A from the client.
 *		3.1) Calculate DH_key
 *		3.2) Compute AES key (SLOW)
 * 4) Send B
 * 5) Receive encrypted message.
 */
