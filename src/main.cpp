#include "dh_params.hpp"
#include "read_args/read_args.hpp"
#include "dh_param_gen/public.hpp"

int main(int argc, char* argv[]) {
	//read args, init general & network fields
	if (dh::parse_args(argc, argv) == 1)
		return 1;

	dh::DH_Params& params = dh::DH_Params::get();
	//if server, pick p & g, generate a
	if (params.is_server()) {
		auto[p, g] = dh::select_public_DH_params();			
		
	} 

	//if client, recieve p & g, generate a, send A
	else {

	}
	
	return 0;
}




	// cpp_int a;
	// a = cpp_int(dh::cleanString(dh::P_6144_BITS));
	// std::cout << a.backend().size() * sizeof(boost::multiprecision::limb_type) * 8 << std::endl;
