#pragma once

#include "dhParams.hpp"

namespace dh {

void DHParams::setGeneralFields(std::optional<std::string> &path) {
	if (path.has_value()) {
		//TODO path validation
		logPath = path.value();
	}

	generalSet = true;
}

void DHParams::setNetworkFields(std::optional<unsigned int> &bitsVal, const std::string &ipVal, const unsigned int portVal) {
	if (bitsVal.has_value()) {
		if (bitsVal.value() % 1024 == 0)
			bits = bitsVal.value();
	}

	ipAddr = ipVal;
	port = portVal;	

	//TODO validate params before setting this true	
	networkSet = true;
}

void DHParams::setDHFields(const Num &pVal, const Num &gVal, const Num &aVal, const Num &AVal, const Num &BVal, const Num &keyVal) {
	p = pVal;
	g = gVal;
	a = aVal;
	A = AVal;
	B = BVal;
	key = keyVal;

	//TODO validate params before setting this true
	dhSet = true;
}

}
