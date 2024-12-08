//param parsing
#include "logger.hpp"
#include "networking/server.hpp"
#include "read_args/read_args.hpp"

//dh param selection
#include "dh_param_gen/public.hpp"
#include "dh_param_gen/private.hpp"

//socket
#include "networking/server.hpp"
#include "networking/client.hpp"
#include "networking/message_formatting.hpp"

//namespace stuff
#include "dh_params.hpp"

int main(int argc, char* argv[]) {
	//read args, init general & network fields
	if (dh::parse_args(argc, argv) == 1)
		return 1;

	dh::DH_Params& params = dh::DH_Params::get();
	dh::Logger& log = dh::Logger::get();
	std::optional<std::string> logPath = params.get_path();
	if (logPath)
		log.initialize(logPath.value());

	if (params.is_server()) {
		//if server, pick p & g, pick private exponent, calculate A=g^a
		auto[p, g] = dh::select_public_DH_params();			
		cpp_int b  = dh::generate_a();
		cpp_int B  = dh::generate_A(p, g, b);
		std::string p_hex = dh::itoh(p);
		std::string g_hex = dh::itoh(g);
		std::string B_hex = dh::itoh(B);

		//create socket
		int socket = dh::create_server();
		if (socket < 0) {
			dh::server_teardown(socket);
			return 1;
		} 
		
		//accept client connection
		int client = dh::accept_client();
		if (client < 0) {
			dh::server_teardown(socket, client);
			return 1;
		}

		cpp_int key = dh::establish_DH_key(p_hex, g_hex, B_hex);
		
	} 

	else {
		//if client, receive p & g, generate a, send A
		int socket = dh::create_client();
		if (socket < 0) {
			dh::client_teardown(socket);
			return 1;
		}

		//recieve p&g from server
		cpp_int p, g;
		if (dh::recv_dh_pub(p, g) < 0) {
			dh::client_teardown(socket);
			return 1;
		}



	}
	
	return 0;
}




	// cpp_int a;
	// a = cpp_int(dh::cleanString(dh::P_6144_BITS));
	// std::cout << a.backend().size() * sizeof(boost::multiprecision::limb_type) * 8 << std::endl;
