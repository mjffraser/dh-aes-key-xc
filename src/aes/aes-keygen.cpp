#include "aes/aes-keygen.hpp"
#include "aes/internal/iv-formatting.hpp"
#include "logger.hpp"

#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/params.h>
#include <openssl/core_dispatch.h>
#include <openssl/core_names.h>

namespace dh {

int aesKeygen(DHParams   const& dh_v,
              AESParams&        aes_v) {
  Logger&       log = Logger::get();
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
	std::stringstream ss; ss << std::hex << dh_v.dh_key;
	pw = ss.str();

	//salt
	std::string salt_hex = "AES-keygen-salt";

	//other
	uint64_t scrypt_N = std::pow(2,20);
	uint32_t scrypt_R = 8;
	uint32_t scrypt_P = 1;
	
	*p++ = OSSL_PARAM_construct_octet_string (OSSL_KDF_PARAM_PASSWORD, (void*)pw.c_str(),				pw.size());
	*p++ = OSSL_PARAM_construct_octet_string (OSSL_KDF_PARAM_SALT,     (void*)salt_hex.c_str(), salt_hex.size());
	*p++ = OSSL_PARAM_construct_uint64       (OSSL_KDF_PARAM_SCRYPT_N, &scrypt_N);
	*p++ = OSSL_PARAM_construct_uint32       (OSSL_KDF_PARAM_SCRYPT_R, &scrypt_R);
	*p++ = OSSL_PARAM_construct_uint32       (OSSL_KDF_PARAM_SCRYPT_P, &scrypt_P);
	*p = OSSL_PARAM_construct_end();
	if (EVP_KDF_derive(kctx, out, sizeof(out), params) <= 0) {
		log.err("Cannot perform scrypt.");
		exit(1); 
	}

  std::memcpy(aes_v.aes_key, &out[0], 32);
  return formCppInt(aes_v.aes_iv, &out[32], 12);
}

} //dh
