#include "readArgs/readArgs.hpp"

int main(int argc, char* argv[]) {
	//read args, init general & network fields
	if (dh::parseArgs(argc, argv) == 1)
		return 1;

	//if server, pick p & g, generate a
	

	//establish socket as server/client
	
	
	//if client, recieve p & g 
	

	//if client, generate a 
	

	//establish key & send message

	return 0;
}
