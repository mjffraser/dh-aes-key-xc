#pragma once

#include "dhParams.hpp"

namespace dh {

void DHParams::setGeneralFields(std::optional<std::string>& path, std::optional<bool>& db, std::optional<bool>& vp) {
	//no changing values after they're set
	if (generalSet)
		return;
	
	if (path)
		logPath = path.value();
	if (db)
		debugFlag = db.value();
	if (vp)
		vettedPrimes = vp.value();

	generalSet = true;
}

void DHParams::setNetworkFields(bool serverFlag, std::optional<unsigned int>& bitsAmount, std::optional<std::string>& ip, std::optional<unsigned int>& portNo) {
	//no changing values after they're set
	if (networkSet) 
		return;

	server = serverFlag;
	if (bitsAmount)
		bits = bitsAmount.value();
	if (ip)
		ipAddr = ip.value();
	if (portNo)
		port = portNo.value();

	networkSet = true;
}

void DHParams::setPublicDHFields(const cpp_int& p, const cpp_int& g) {
	//no changing values after they're set
	if (dhPublicSet)
		return;

	_p = p;
	_g = g;

	dhPublicSet = true;
}

void DHParams::setPrivateDHFields(const cpp_int& a, const cpp_int& A, const cpp_int& B, const cpp_int& key) {
	//no changing values after they're set
	if (dhPrivateSet)
		return;

	_a   = a;
	_A   = A;
	_B   = B;
	_key = key;
}

}
