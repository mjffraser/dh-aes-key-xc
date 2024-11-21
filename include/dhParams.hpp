#pragma once

#include "num.hpp"

#include <optional>
#include <string>

namespace dh {

class DHParams {
private:
	//general
	bool				generalSet = false;
	std::string logPath;

	//networking fields
	bool				 networkSet = false;
	unsigned int bits = 2048; //p bits
	std::string  ipAddr; //ip for connection/listening
	unsigned int port;
	
	//dh fields
	bool dhSet = false;
	Num p;   //large prime
	Num g;	 //primitive root of p 
	Num a;	 //secret key choice 
	Num A;	 //g^a mod p 
	Num B;	 //recieved g^b from other side
	Num key; //B^a mod p 

	//function to guard against a parameters use if it wasn't initialized properly
	template <typename Param>
	static std::optional<Param> retOkay(Param &field, bool setFlag) {
		if (setFlag == true)
			return field;
		else 
			return std::nullopt;
	}

public:
	DHParams() {}
	void setGeneralFields(std::optional<std::string> &path);
	void setNetworkFields(std::optional<unsigned int> &bitsVal, const std::string &ipVal, const unsigned int portVal);
	void setDHFields(const Num &pVal, const Num &gVal, const Num &aVal, const Num &AVal, const Num &BVal, const Num &keyVal);

	std::optional<std::string>  getPath() { return retOkay(logPath, generalSet); }
	std::optional<unsigned int> getBits() { return retOkay(bits,    networkSet); }
	std::optional<std::string>  getIP()   { return retOkay(ipAddr,  networkSet); }
	std::optional<unsigned int> getPort() { return retOkay(port,    networkSet); }
	std::optional<Num>					dh_p()    { return retOkay(p,       dhSet);      }
	std::optional<Num>					dh_g()    { return retOkay(g,       dhSet);      }
	std::optional<Num>					dh_a()    { return retOkay(a,       dhSet);      }
	std::optional<Num>					dh_A()    { return retOkay(A,       dhSet);      }
	std::optional<Num>					dh_B()    { return retOkay(B,       dhSet);      }
	std::optional<Num>					dh_key()  { return retOkay(key,     dhSet);      }

};

}
