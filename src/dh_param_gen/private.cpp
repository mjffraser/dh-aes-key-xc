#include "dh_param_gen/private.hpp"
#include "dh_param_gen/internal/random_util.hpp"
#include "logger.hpp"

namespace dh {

cpp_int generate_a() {
	DH_Params& params = DH_Params::get();
	Logger&		 log    = Logger::get();

	//We choose our secret exponent a [1, 2^256]
	//b/c of NFS's we don't really gain anything by making a any bigger.
	cpp_int lower(2);									 //lower bound
	cpp_int upper(2); upper = boost::multiprecision::pow(upper, 256); //upper bound
	cpp_int a = rand_between(lower, upper);
	if (params.debug())
		log.append_to_log("[LOG] Using a=" + a.str());

	return a;
}

cpp_int generate_A(cpp_int& p, cpp_int& g, cpp_int& a) {
	DH_Params& params = DH_Params::get();
	Logger&		 log		= Logger::get();

	cpp_int A = powm(g, a, p);
	if (params.debug())
		log.append_to_log("[LOG] Using A=" + A.str());
	return A;
}

cpp_int generate_key(cpp_int& p, cpp_int& a, cpp_int& B) {
	DH_Params& params = DH_Params::get();
	Logger&		 log		= Logger::get();

	cpp_int key = powm(B, a, p);
	if (params.debug())
		log.append_to_log("[LOG] Using key=" + key.str());
	return key;
}

}
