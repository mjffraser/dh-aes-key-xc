#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/number.hpp>
#include <string>

namespace dh {

using namespace boost::multiprecision;

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ConfigParams
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> General configs that are needed in most places in this program.
 *
 * Fields:
 * -> debug:
 *    If messages should be logged to a file.
 * -> quiet:
 *    If warning and info messages should be suppressed.
 * -> verbose:
 *    If all messages should be printed to console.
 * -> log_path:
 *    The path of the file to log to.
 * -> server:
 *    If this is a server or client instance.
 * -> bits:
 *    Bits to use for prime p. Available settings are 1536, 2048, 3072, 4096,
 *    6144, 8192.
 * -> ip_addr:
 *    IPv4 address to connect to as a client.
 * -> port:
 *    The port to bind/connect to.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
struct ConfigParams {
  bool        debug    = false;
  bool        quiet    = false;
  bool        verbose  = false;
  std::string log_path = "log";

  //networking
  bool        server  = false;
  uint16_t    bits    = 2048;
  std::string ip_addr = "127.0.0.1";
  uint16_t    port    = 65000;
};

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * DHParams
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> The various values needed for the Diffie-Hellman key agreement.
 * 
 * Fields:
 * -> p:
 *    The public prime.
 * -> g:
 *    The generator g for prime p.
 * -> a:
 *    The secret exponent 1 < a < p-1.
 * -> A:
 *    g^a mod p
 * -> B:
 *    g^b mob p where b is the other side's secret exponent.
 * -> dh_key:
 *    B^a mod p === g^ab mod p
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
struct DHParams {
  //public dh values
  cpp_int p = -1;
  cpp_int g = -1;

  //private dh values
  cpp_int a = -1;
  cpp_int A = -1;
  cpp_int B = -1;
  
  //session key
  cpp_int dh_key = -1;
};

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * AESParams
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> The values needed for AES encryption/decryption of messages.
 *
 * Fields:
 * -> aes_key:
 *    The key produced by feeding dh_key into scrypt.
 * -> aes_iv:
 *    The nonce to use for AES-GCM. When dh_key is passed through scrypt extra
 *    bytes are drawn past the key to use as the IV.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
struct AESParams {
  std::string aes_key;
  cpp_int     aes_iv = -1;
};

}
