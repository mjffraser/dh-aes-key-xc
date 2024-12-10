#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/number.hpp>

#include <string>


using namespace boost::multiprecision;

namespace dh {

/* 
 * Params:
 * Object that houses values needed for DH 
 * communication over sockets, as well as 
 * the AES key used to encrypt messages. 
 *
 * Defaults are listed beside any fields
 * that have one.
 *
 * General fields:
 * - debug (false)
 *	 > Should messages be logged to a file.
 * - quiet (false)
 *   > Should feedback messages to the console
 *		 be suppressed.
 * - verbose (false)
 *   > Should all messages be printed to console.
 * - log_path ("./log")
 *   > The path of the file to log to if debug
 *	   is true.
 *
 * Network fields:
 * - server (MUST BE SET)
 *   > Is this server or client. True for server.
 * - bits (2048)
 *   > Bits to use for prime p. Available settings
 *	   are 1536, 2048, 3072, 4096, 6144, 8192.
 *	   Anything above 2048 is excessive, as DH is 
 *	   the computational bottleneck for security at
 *	   ~110-160 bits for 2048, and ~190-310 bits for 
 *	   8192. 
 * - > ip_addr (LOOPBACK_ADDR)
 *		 IPv4 address to either bind to as server, or
 *		 connect to as client.
 * - > port (65000)
 *     The port to use/connect to.
 *
 * Public fields:
 * - p (2048-bit p in vetted_primes)
 *	 > The prime p to use for DH.
 * - g (2)
 *   > The generator g to use for p.
 *
 * Private fields:
 * - a 
 *   > The secret exponent 1 < a < p-1.
 * - A 
 *   > g^a mod p 
 * - B 
 *   > g^b mod p where b is the other
 *   side's secret exponent.
 * - dh_key
 *   > B^a mod p  
 *
 * AES fields:
 * - aes_key
 *   > The AES key produced by feeding
 *     dh_key into scrypt.
 * - aes_iv
 *   > A nonce to use for AES-GCM. NEVER 
 *   TO BE REUSED or the XOR of plaintexts 
 *   is revealed. This number should be 
 *   initalized with a fixed-width 96-bit
 *   IV produced by aes_keygen, and then 
 *   incremented for each encryption.
 */

struct Params {
	//general fields
	bool				 debug         = false;
	bool				 quiet				 = false;
	bool				 verbose			 = false;
	std::string  log_path      = "log";

	//network fields 
	bool				 server        = false;
	unsigned int bits          = 2048;
	std::string  ip_addr       = "127.0.0.1";
	unsigned int port          = 65000;

	//public dh fields
	cpp_int p = -1;
	cpp_int g = -1;

	//private dh fields (A & B are still public, but are session-specific so they're here)
	cpp_int a = -1;
	cpp_int A = -1;
	cpp_int B = -1;
	cpp_int dh_key = -1; 

	//aes fields
	std::string aes_key;
	cpp_int     aes_iv = -1; 
};

}
