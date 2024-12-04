#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/number.hpp>

#include <optional>
#include <string>


using namespace boost::multiprecision;

namespace dh {

/* 
 * DH_Params:
 * Class object that houses values needed for DH communication over sockets.
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
 *		Public value recieved from server/client on other end.
 * -> _key 
 *		Secret key to encrypt messages with. Derived from B^a mod p. Should not be disclosed.
 */

class DH_Params {
private:
	//enforce single instance
	DH_Params()	 {}
	~DH_Params() {}

	//general
	bool				general_set   = false;
	bool				debug_flag    = false;
	bool				vetted_primes = true;
	std::string log_path      = "";

	//networking fields
	bool				 network_set  = false;
	bool				 server       = false;
	unsigned int bits         = 2048;
	std::string  ip_addr      = "127.0.0.1";
	unsigned int port         = 8080;
	
	//public dh fields
	//these are decided by server, and must be recieved over socket by client
	bool dh_public_set = false;
	cpp_int _p;   
	cpp_int _g;		
	
	//private dh fields
	bool dh_private_set = false;
	cpp_int _a;		
	cpp_int _A;		
	cpp_int _B;		
	cpp_int _key; 

	//function to guard against a parameters use if it wasn't initialized properly
	template <typename Param>
	static std::optional<Param> ret_okay(Param& field, bool set_flag) {
		if (set_flag == true)
			return field;
		else 
			return std::nullopt;
	}


public:
	//GET INSTANCE HERE
	static DH_Params& get() {
		static DH_Params instance;
		return instance;
	}
	
	//COPY & MOVE
	DH_Params(DH_Params const&)			 = delete;
	void operator=(DH_Params const&) = delete;

	void set_general_fields(std::optional<std::string>&	 path, 
													std::optional<bool>&				 db, 
													std::optional<bool>&				 vp);

	void set_network_fields(bool												 server_flag, 
													std::optional<unsigned int>& bit_amount, 
													std::optional<std::string> & ip, 
													std::optional<unsigned int>& port_no);

	void set_public_DH_fields(const cpp_int& p, const cpp_int& g);
	void set_private_DH_fields(const cpp_int& a, const cpp_int& A, const cpp_int& B, const cpp_int& key);

	bool												debug()             { return debug_flag;												 }
	bool												is_server()         { return server;														 }
	bool												use_vetted_primes() { return vetted_primes;											 }
	std::optional<std::string>  get_path()          { return ret_okay(log_path, general_set);    }
	std::optional<unsigned int> get_bits()          { return ret_okay(bits,     network_set);    }
	std::optional<std::string>  get_IP()            { return ret_okay(ip_addr,  network_set);    }
	std::optional<unsigned int> get_port()          { return ret_okay(port,     network_set);		 }
	std::optional<cpp_int>			p()                 { return ret_okay(_p,       dh_public_set);  }
	std::optional<cpp_int>			g()                 { return ret_okay(_g,       dh_public_set);  }
	std::optional<cpp_int>			a()                 { return ret_okay(_a,       dh_private_set); }
	std::optional<cpp_int>			A()                 { return ret_okay(_A,       dh_private_set); }
	std::optional<cpp_int>			B()                 { return ret_okay(_B,       dh_private_set); }
	std::optional<cpp_int>			key()               { return ret_okay(_key,     dh_private_set); }

};

}
