#pragma once

#include "num.hpp"

#include <optional>
#include <string>

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
	bool				generalSet  = false;
	bool				debugFlag		= false;
	std::string logPath     = "";

	//networking fields
	bool				 networkSet = false;
	unsigned int bits       = 2048;
	std::string  ipAddr     = "127.0.0.1";
	unsigned int port       = 8080;
	
	//public dh fields
	//these are decided by server, and must be recieved over socket by client
	bool dhPublicSet = false;
	Num _p;   
	Num _g;		
	
	//private dh fields
	bool dhPrivateSet = false;
	Num _a;		
	Num _A;		
	Num _B;		
	Num _key; 

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

	void setGeneralFields(std::optional<std::string>& path, std::optional<bool>& db);
	void setNetworkFields(std::optional<unsigned int>& bitAmount, std::optional<std::string> & ip, std::optional<unsigned int>& portNo);
	void setPublicDHFields(const Num& p, const Num& g);
	void setPrivateDHFields(const Num& a, const Num& A, const Num& B, const Num& key);

	bool												debug()   { return debugFlag;                      }
	std::optional<std::string>  getPath() { return retOkay(logPath, generalSet);   }
	std::optional<unsigned int> getBits() { return retOkay(bits,    networkSet);   }
	std::optional<std::string>  getIP()   { return retOkay(ipAddr,  networkSet);   }
	std::optional<unsigned int> getPort() { return retOkay(port,    networkSet);   }
	std::optional<Num>					p()       { return retOkay(_p,      dhPublicSet);  }
	std::optional<Num>					g()       { return retOkay(_g,      dhPublicSet);  }
	std::optional<Num>					a()       { return retOkay(_a,      dhPrivateSet); }
	std::optional<Num>					A()       { return retOkay(_A,      dhPrivateSet); }
	std::optional<Num>					B()       { return retOkay(_B,      dhPrivateSet); }
	std::optional<Num>					key()     { return retOkay(_key,    dhPrivateSet); }

};

}
