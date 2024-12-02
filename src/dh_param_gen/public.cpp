#include "dh_param_gen/public.hpp"
#include "dh_param_gen/internal/vetted_primes.hpp"
#include "logger.hpp"


namespace dh {

std::pair<cpp_int, cpp_int> select_public_DH_params() {
	//TODO: ADD RANDOM P GEN
	
	DH_Params& params = DH_Params::get();
	Logger& log = Logger::get(); 

	unsigned int bits = 0;
	auto b = params.get_bits();	
	if (b) 
		bits = b.value();
	else {
		log.append_to_log("[ERR] BITS NOT INITIALIZED IN PARAMETERS.");
		throw std::invalid_argument("Missing bits val. See log.");
	}
		
	

	cpp_int p, g;
	if (params.use_vetted_primes() || true) { //REMOVE WITH TODO
		p = cpp_int(get_vetted_p(bits));
		g = cpp_int(get_vetted_g());
	} else {
		//TODO GENERATE P
	}

	return (std::make_pair(p, g));	
}

}
