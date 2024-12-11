#include "dh_param_gen/public.hpp"
#include "dh_param_gen/internal/vetted_primes.hpp"
#include "logger.hpp"


namespace dh {

std::pair<cpp_int, cpp_int> select_public_DH_params(Params& params) {
	//TODO: ADD RANDOM P GEN
	Logger& log = Logger::get(); 

	unsigned int bits = params.bits;	
	cpp_int p, g;
	p = cpp_int(get_vetted_p(bits));
	g = cpp_int(get_vetted_g());

	if (bits != (p.backend().size() * sizeof(boost::multiprecision::limb_type) * 8)) {
		log.append_to_log("[WARN] Entry of" + std::to_string(bits) + " bits appears invalid. Using default of 2048 instead.");
	}

	return (std::make_pair(p, g));	
}

}
