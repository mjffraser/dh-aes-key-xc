#include "networking/internal/encrypt/aes_gcm.hpp"
#include "dh_params.hpp"
#include "logger.hpp"
#include <openssl/evp.h>

namespace dh {

void err_out(const std::string& msg, Logger& log, Params& param) {
	if (param.debug)
		log.append_to_log(msg);
	exit(1);
}

int encrypt(unsigned char* plaintext,
						int						 plaintext_len,
						unsigned char* key,
						unsigned char* iv,
						unsigned char* ciphertext,
						unsigned char* tag,
						Params&				 params) {
	
	Logger& log = Logger::get();
	EVP_CIPHER_CTX* ctx;
	int len;
	int ciphertext_len;

	//init contex
	if (!(ctx = EVP_CIPHER_CTX_new())) 
		err_out("[ERR] Couldn't init context.", log, params);	

	//init ecryption operation
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		err_out("[ERR] Couldn't init AES-GCM.", log, params);

	//set iv length (96-bits)
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL))
		err_out("[ERR] Couldn't set IV length for AES.", log, params);

	//init key & iv
	if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv))
		err_out("[ERR] Couldn't set key and IV for AES.", log, params);

	//encrypt plaintext to ciphertext
	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		err_out("[ERR] Couldn't encrypt plaintext.", log, params);
	ciphertext_len += len;

	//finalize
	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext+len, &len))
		err_out("[ERR] Couldn't finalize encryption", log, params);

	//get tag 
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
		err_out("[ERR] Could not retrieve encryption tag.", log, params);

	//clean up
	EVP_CIPHER_CTX_free(ctx);
	return ciphertext_len;
}


int decrypt(unsigned char* ciphertext,
						int						 ciphertext_len,
						unsigned char* tag,
						unsigned char* key,
						unsigned char* iv,
						unsigned char* plaintext,
						Params&				 params) {
	
	Logger& log = Logger::get();
	EVP_CIPHER_CTX* ctx;
	int len;
	int plaintext_len;
	int ret;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		err_out("[ERR] Couldn't init context.", log, params);

	//init decryption operation
	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1)
		err_out("[ERR] Couldn't init AES-GCM decryption.", log, params);

	//set iv length (96-bits)
	if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL) != 1)
		err_out("[ERR] Couldn't set IV length for AES.", log, params);

	//init key & iv
	if (EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv) != 1)
		err_out("[ERR] Couldn't set key and IV for AES.", log, params);

	//decrypt ciphertext to plaintext
	if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
		err_out("[ERR] Couldn't decrypt ciphertext.", log, params);

	plaintext_len = len;

	if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
		err_out("[ERR] Couldn't set tag.", log, params);

	//finalize
	ret = EVP_DecryptFinal_ex(ctx, plaintext+len, &len);

	//clean up
	EVP_CIPHER_CTX_free(ctx);

	if (ret > 0) {
		plaintext_len += len;
		return plaintext_len;
	} else {
		return -1;
	}
}

}
