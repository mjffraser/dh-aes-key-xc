#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/number.hpp>

#include <string>


using namespace boost::multiprecision;

namespace dh {

/* 
 * Params:
 * Object that houses values needed for DH communication over sockets, as well
 * as the AES key used to encrypt messages.
 *
 * General:
 * -> debug_flag (default FALSE)
 *		Denotes debugging mode for any functions asking to call logger.
 * -> log_path (default <current working dir>)
 *		Where to create the log file.
 *
 * Networking:
 * -> bits (default 2048)
 *		How large p should be. Recommendation is 256-byte prime, hence the default.
 * -> ip_addr (default LOOPBACK)
 *		IP address for socket connection.
 * -> port (default 8080)
 *		Port to bind socket to.
 *
 * DH fields: (Underscores are used so the getters can have less characters)
 * -> _p
 *		Large prime to generate secret keys with.
 * -> _g
 *		Primitive root of p.
 * -> _a
 *		Secret value for this server/client instance. Should not be disclosed.
 * -> _A
 *		Public value for this server/client instance. Derived from g^a mod p.
 * -> _B
 *		Public value received from server/client on other end.
 * -> _key 
 *		Secret key to encrypt messages with. Derived from B^a mod p. Should not be disclosed.
 */

struct Params {
	//general fields
	bool				 debug         = false;
	bool				 vetted_primes = false;
	std::string  log_path      = "log";

	//network specific
	bool				 server        = false;
	unsigned int bits          = 2048;
	std::string  ip_addr       = "127.0.0.1";
	unsigned int port          = 65000;

	//the public dh prime and prim root
	cpp_int p = -1;
	cpp_int g = -1;

	//other values used for dh
	cpp_int a = -1;
	cpp_int A = -1;
	cpp_int B = -1;
	cpp_int dh_key = -1; 

	//the derived aes key
	std::string aes_key;
};

}
