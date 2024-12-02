#include "dhParams.hpp"
#include "readArgs/readArgs.hpp"
#include "dhParamGen/public.hpp"

int main(int argc, char* argv[]) {
	//read args, init general & network fields
	if (dh::parseArgs(argc, argv) == 1)
		return 1;

	dh::DHParams& params = dh::DHParams::get();
	//if server, pick p & g, generate a
	if (params.isServer()) {
		auto[p, g] = dh::selectPublicDHParams();			
		
	} 

	//if client, recieve p & g, generate a, send A
	else {

	}
	
	return 0;
}




	// cpp_int a;
	// a = cpp_int(dh::cleanString(dh::P_6144_BITS));
	// std::cout << a.backend().size() * sizeof(boost::multiprecision::limb_type) * 8 << std::endl;
