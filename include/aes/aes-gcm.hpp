#pragma once

#include "dh-aes-params.hpp"

#include <sys/types.h>

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aesEncrypt
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Encrypts a plaintext with AES_GCM, storing the resulting ciphertext and
 *    tag. The IV and key is drawn from the AESParams object. 
 *
 *    The IV ***MUST*** be incremented outside this function.
 *
 * Takes:
 * -> plaintext:
 *    The message to encrypt.
 * -> ciphertext:
 *    The container to store the ciphertext in. Should be the same size as the
 *    plaintext.
 * -> tag:
 *    The tag produced.
 * -> data_len:
 *    The length of the plaintext/ciphertext.
 * -> aes_v:
 *    The AESParams container with the IV and key for encryption.
 *   
 * Returns:
 * -> On success:
 *    The length of the ciphertext produced.
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int aesEncrypt(unsigned char  const* const  plaintext,
               unsigned char*               ciphertext,
               unsigned char*               tag,
               size_t                const  data_len,
               AESParams             const& aes_v);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aesDecrypt
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Decrypts a ciphertext with AES_GCM, storing the resulting plaintext. The
 *    IV and key is drawn from the AESParams object. 
 *
 *    The IV ***MUST*** be incremented outside this function.
 *
 * Takes:
 * -> plaintext:
 *    The container to store the plaintext in. Should be the same size as the
 *    ciphertext.
 * -> ciphertext:
 *    The ciphertext to decrypt.
 * -> tag:
 *    The tag for decryption.
 * -> data_len:
 *    The length of the plaintext/ciphertext.
 * -> aes_v:
 *    The AESParams container with the IV and key for encryption.
 *   
 * Returns:
 * -> On success:
 *    The length of the plaintext decrypted.
 * -> On failure:
 *    -1
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int aesDecrypt(unsigned char*        plaintext,
               unsigned char* const  ciphertext,
               unsigned char* const  tag,
               size_t         const  data_len,
               AESParams      const& aes_v);

} //dh
