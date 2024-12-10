#include "networking/internal/encrypt/keygen.hpp"
#include "dh_params.hpp"
#include "logger.hpp"

#include <cstdlib>
#include <error.h>
#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/params.h>
#include <openssl/core_dispatch.h>
#include <openssl/core_names.h>

namespace dh {



//https://docs.openssl.org/3.4/man7/EVP_KDF-SCRYPT/#examples
std::pair<std::string, std::string> aes_keygen(cpp_int& dh_shared_key, Params& dh_params) {
	Logger&				log						= Logger::get();
	EVP_KDF*			kdf;
	EVP_KDF_CTX*	kctx;
	unsigned char out[32+12]; //256-bit AES key, 12-byte IV
	OSSL_PARAM		params[6];
	OSSL_PARAM*   p = params;

	kdf = EVP_KDF_fetch(NULL, "SCRYPT", NULL);
	kctx = EVP_KDF_CTX_new(kdf);
	EVP_KDF_free(kdf);

	//password
	std::string pw;
	std::stringstream ss; ss << std::hex << dh_shared_key;
	pw = ss.str();

	//salt
	std::string salt_hex = "AES-keygen-salt";

	//other
	uint64_t scrypt_N = std::pow(2, 14);
	uint32_t scrypt_R = 8;
	uint32_t scrypt_P = 1;
	
	*p++ = OSSL_PARAM_construct_octet_string (OSSL_KDF_PARAM_PASSWORD, (void*)pw.c_str(),				pw.size());
	*p++ = OSSL_PARAM_construct_octet_string (OSSL_KDF_PARAM_SALT,     (void*)salt_hex.c_str(), salt_hex.size());
	*p++ = OSSL_PARAM_construct_uint64       (OSSL_KDF_PARAM_SCRYPT_N, &scrypt_N);
	*p++ = OSSL_PARAM_construct_uint32       (OSSL_KDF_PARAM_SCRYPT_R, &scrypt_R);
	*p++ = OSSL_PARAM_construct_uint32       (OSSL_KDF_PARAM_SCRYPT_P, &scrypt_P);
	*p = OSSL_PARAM_construct_end();
	if (EVP_KDF_derive(kctx, out, sizeof(out), params) <= 0) {
		log.append_to_log("[ERR] Cannot perform scrypt.");
		exit(1); 
	}

	std::string aes_key(&out[0], &out[0]+32);
	std::string aes_iv(&out[32], &out[32]+12);

	return std::make_pair(aes_key, aes_iv);
}

}
