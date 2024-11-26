#include "readArgs/readArgs.hpp"
#include "dhParams.hpp"
#include "dhParamGen/internal/vettedPrimes.hpp"

int main(int argc, char* argv[]) {
	//read args, init general & network fields
	if (dh::parseArgs(argc, argv) == 1)
		return 1;

	//if server, pick p & g, generate a
	
	cpp_int a;
	a = cpp_int(dh::cleanString(dh::P_6144_BITS));
	std::cout << a.backend().size() * sizeof(boost::multiprecision::limb_type) * 8 << std::endl;

	//establish socket as server/client
	
	
	//if client, recieve p & g 
	

	//if client, generate a 
	

	//establish key & send message

	return 0;
}
