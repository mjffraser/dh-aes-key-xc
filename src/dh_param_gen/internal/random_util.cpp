#include "dh_param_gen/internal/random_util.hpp"
#include "logger.hpp"

#include <boost/nondet_random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace dh {

static boost::random_device rd;

cpp_int rand_between(cpp_int& lower, cpp_int& upper) {
	DH_Params& param = DH_Params::get();
	Logger&		 log   = Logger::get();

	//define range
	boost::random::uniform_int_distribution<cpp_int> gen(lower+1, upper-1);
	cpp_int selected = gen(rd);
	if (param.debug())
		log.append_to_log("[LOG] Generated value: " + selected.str());
	return selected;
}

cpp_int rand_prime(unsigned int bits) {
	//TODO
	return cpp_int();
}

}
