#include "encrypt/aes_gcm.hpp"
#include "dh_params.hpp"
#include "logger.hpp"
#include <openssl/evp.h>

namespace dh {

void err_out(const std::string& msg, Logger& log, DH_Params& param) {
	if (param.debug())
		log.append_to_log(msg);
	exit(1);
}

int encrypt(unsigned char* plaintext,
						int						 plaintext_len,
						unsigned char* key,
						unsigned char* iv,
						unsigned char* ciphertext,
						unsigned char* tag) {
	
	Logger& log = Logger::get();
	DH_Params& dh_param = DH_Params::get();
	EVP_CIPHER_CTX* ctx;
	int len;
	int ciphertext_len;

	//init contex
	if (!(ctx = EVP_CIPHER_CTX_new())) 
		err_out("[ERR] Couldn't init context.", log, dh_param);	

	//init ecryption operation
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		err_out("[ERR] Couldn't init AES-GCM.", log, dh_param);

	//set iv length (128-bits)
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
		err_out("[ERR] Couldn't set IV length for AES.", log, dh_param);

	//init key & iv
	if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv))
		err_out("[ERR] Couldn't set key and IV for AES.", log, dh_param);

	//encrypt plaintext to ciphertext
	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		err_out("[ERR] Couldn't encrypt plaintext.", log, dh_param);
	ciphertext_len += len;

	//finalize
	if (EVP_EncryptFinal_ex(ctx, ciphertext+len, &len) != 1)
		err_out("[ERR] Couldn't finalize encryption", log, dh_param);

	//get tag 
	if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag) != 1)
		err_out("[ERR] Could not retrieve encryption tag.", log, dh_param);

	//clean up
	EVP_CIPHER_CTX_free(ctx);
	return ciphertext_len;
}


int decrypt(unsigned char* ciphertext,
						int						 ciphertext_len,
						unsigned char* tag,
						unsigned char* key,
						unsigned char* iv,
						unsigned char* plaintext) {
	
	Logger& log = Logger::get();
	DH_Params& dh_param = DH_Params::get();
	EVP_CIPHER_CTX* ctx;
	int len;
	int plaintext_len;
	int ret;

	if (!(ctx = EVP_CIPHER_CTX_new()))
		err_out("[ERR] Couldn't init context.", log, dh_param);

	//init decryption operation
	if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1)
		err_out("[ERR] Couldn't init AES-GCM decryption.", log, dh_param);

	//set iv length (128-bits)
	if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL) != 1)
		err_out("[ERR] Couldn't set IV length for AES.", log, dh_param);

	//init key & iv
	if (EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv) != 1)
		err_out("[ERR] Couldn't set key and IV for AES.", log, dh_param);

	//decrypt ciphertext to plaintext
	if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
		err_out("[ERR] Couldn't decrypt ciphertext.", log, dh_param);

	plaintext_len = len;

	if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag))
		err_out("[ERR] Couldn't set tag.", log, dh_param);

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
