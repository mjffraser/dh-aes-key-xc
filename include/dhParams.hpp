#pragma once

#include <boost/multiprecision/cpp_int.hpp>

#include <boost/multiprecision/number.hpp>
#include <optional>
#include <string>

using cpp_int = boost::multiprecision::cpp_int;

namespace dh {

/* 
 * DHParams:
 * Class object that houses values needed for DH communication over sockets.
 *
 * General:
 * -> debugFlag (default FALSE)
 *		Denotes debugging mode for any functions asking to call logger.
 * -> logPath (default <current working dir>)
 *		Where to create the log file.
 *
 * Networking:
 * -> bits (default 2048)
 *		How large p should be. Recommendation is 256-byte prime, hence the default.
 * -> ipAddr (default LOOPBACK)
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

class DHParams {
private:
	//enforce single instance
	DHParams()	{}
	~DHParams() {}

	//general
	bool				generalSet   = false;
	bool				debugFlag    = false;
	bool				vettedPrimes = true;
	std::string logPath      = "";

	//networking fields
	bool				 networkSet  = false;
	bool				 server      = false;
	unsigned int bits        = 2048;
	std::string  ipAddr      = "127.0.0.1";
	unsigned int port        = 8080;
	
	//public dh fields
	//these are decided by server, and must be recieved over socket by client
	bool dhPublicSet = false;
	cpp_int _p;   
	cpp_int _g;		
	
	//private dh fields
	bool dhPrivateSet = false;
	cpp_int _a;		
	cpp_int _A;		
	cpp_int _B;		
	cpp_int _key; 

	//function to guard against a parameters use if it wasn't initialized properly
	template <typename Param>
	static std::optional<Param> retOkay(Param& field, bool setFlag) {
		if (setFlag == true)
			return field;
		else 
			return std::nullopt;
	}


public:
	//GET INSTANCE HERE
	static DHParams& get() {
		static DHParams instance;
		return instance;
	}
	
	//COPY & MOVE
	DHParams(DHParams const&)				= delete;
	void operator=(DHParams const&) = delete;

	void setGeneralFields(std::optional<std::string>&	 path, 
												std::optional<bool>&				 db, 
												std::optional<bool>&				 vp);

	void setNetworkFields(bool												 serverFlag, 
												std::optional<unsigned int>& bitAmount, 
												std::optional<std::string> & ip, 
												std::optional<unsigned int>& portNo);

	void setPublicDHFields(const cpp_int& p, const cpp_int& g);
	void setPrivateDHFields(const cpp_int& a, const cpp_int& A, const cpp_int& B, const cpp_int& key);

	bool												debug()           { return debugFlag;                      }
	bool												isServer()        { return server;												 }
	bool												useVettedPrimes() { return vettedPrimes;									 }
	std::optional<std::string>  getPath()         { return retOkay(logPath, generalSet);   }
	std::optional<unsigned int> getBits()         { return retOkay(bits,    networkSet);   }
	std::optional<std::string>  getIP()           { return retOkay(ipAddr,  networkSet);   }
	std::optional<unsigned int> getPort()         { return retOkay(port,    networkSet);   }
	std::optional<cpp_int>			p()               { return retOkay(_p,      dhPublicSet);  }
	std::optional<cpp_int>			g()               { return retOkay(_g,      dhPublicSet);  }
	std::optional<cpp_int>			a()               { return retOkay(_a,      dhPrivateSet); }
	std::optional<cpp_int>			A()               { return retOkay(_A,      dhPrivateSet); }
	std::optional<cpp_int>			B()               { return retOkay(_B,      dhPrivateSet); }
	std::optional<cpp_int>			key()             { return retOkay(_key,    dhPrivateSet); }

};

}
