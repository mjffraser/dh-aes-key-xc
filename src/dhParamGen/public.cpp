#include "dhParamGen/public.hpp"
#include "dhParamGen/internal/vettedPrimes.hpp"
#include "logger.hpp"


namespace dh {

std::pair<cpp_int, cpp_int> selectPublicDHParams() {
	//TODO: ADD RANDOM P GEN
	
	DHParams& params = DHParams::get();
	Logger& log = Logger::get(); 

	unsigned int bits = 0;
	auto b = params.getBits();	
	if (b) 
		bits = b.value();
	else {
		log.appendToLog("[ERR] BITS NOT INITIALIZED IN PARAMETERS.");
		throw std::invalid_argument("Missing bits val. See log.");
	}
		
	

	cpp_int p, g;
	if (params.useVettedPrimes() || true) { //REMOVE WITH TODO
		p = cpp_int(getVettedP(bits));
		g = cpp_int(getVettedG());
	} else {
		//TODO GENERATE P
	}

	return (std::make_pair(p, g));	
}

}
