//param parsing
#include "read_args/read_args.hpp"

//dh param selection
#include "dh_param_gen/public.hpp"
#include "dh_param_gen/private.hpp"

//socket

//namespace stuff
#include "dh_params.hpp"

int main(int argc, char* argv[]) {
	//read args, init general & network fields
	if (dh::parse_args(argc, argv) == 1)
		return 1;

	dh::DH_Params& params = dh::DH_Params::get();
	if (params.is_server()) {
		//if server, pick p & g, generate a
		auto[p, g] = dh::select_public_DH_params();			
		cpp_int a  = dh::generate_a();
		
		//then open socket to listen for client
		
	} 

	else {
		//if client, recieve p & g, generate a, send A

	}
	
	return 0;
}




	// cpp_int a;
	// a = cpp_int(dh::cleanString(dh::P_6144_BITS));
	// std::cout << a.backend().size() * sizeof(boost::multiprecision::limb_type) * 8 << std::endl;
