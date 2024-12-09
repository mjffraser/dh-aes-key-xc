#include "dh_param_gen/private.hpp"
#include "rand/random_util.hpp"
#include "logger.hpp"
#include <boost/multiprecision/cpp_int/cpp_int_config.hpp>

namespace dh {

cpp_int generate_a(Params& params) {
	Logger&	log = Logger::get();

	if (params.p < 0) {
		log.append_to_log("[ERR] Prime p is not initialized.");
		return -1;
	}
	int p_bits = params.p.backend().size() * sizeof(boost::multiprecision::limb_type) * 8;

	//We choose our secret exponent a [1, 2^256]
	//b/c of NFS's we don't really gain anything by making a any bigger.
	cpp_int lower(2);
	cpp_int upper(2);
	upper = boost::multiprecision::pow(upper, p_bits); 
	upper -= 1; //avoid case p-1
	cpp_int a = rand_between(lower, upper);


	log.append_to_log("[LOG] Using a=" + a.str());

	return a;
}

cpp_int generate_A(Params& params) {
	Logger&	log = Logger::get();
	if (params.p < 0 ||
			params.g < 0 ||
			params.a < 0) {
		log.append_to_log("[ERR] Param p or g or a is missing.");
		return -1;
	}


	cpp_int A = powm(params.g, params.a, params.p);
	log.append_to_log("[LOG] Using A=" + A.str());
	return A;
}

cpp_int generate_key(Params& params) {
	Logger&	log = Logger::get();
	if (params.p < 0 ||
			params.a < 0 ||
			params.B < 0) {
		log.append_to_log("[ERR] Param p or a or B is missing.");
		return -1;
	}

	cpp_int key = powm(params.B, params.a, params.p);
	log.append_to_log("[LOG] Using key=" + key.str());
	return key;
}

}
