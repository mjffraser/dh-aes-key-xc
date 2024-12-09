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
		cpp_int a  = dh::generate_a();
		cpp_int A  = dh::generate_A(p, g, a);

		params.p = p;
		params.g = g;
		params.a = a;
		params.A = A;

		//create socket
		int server = dh::create_server();
		if (server < 0) {
			dh::server_teardown(server);
			return 1;
		} 
		
		//accept client connection
		int client = dh::accept_client(server);
		if (client < 0) {
			dh::server_teardown(server, client);
			return 1;
		}

		if (dh::send_p_g(client) < 0) {
			dh::server_teardown(server, client);
			return 1;
		}

		dh::server_teardown(server, client);
	} 

	else {
		//if client, receive p & g, generate a, send A
		int socket = dh::create_client();
		if (socket < 0) {
			dh::client_teardown(socket);
			return 1;
		}

		if (dh::connect_to_server(socket) != 0) {
			dh::client_teardown(socket);
			return 1;
		}

		//recieve p&g from server
		cpp_int p, g;
		if (dh::recv_dh_pub(socket, p, g) < 0) {
			dh::client_teardown(socket);
			return 1;
		}


		dh::client_teardown(socket);
	}
	
	return 0;
}




	// cpp_int a;
	// a = cpp_int(dh::cleanString(dh::P_6144_BITS));
	// std::cout << a.backend().size() * sizeof(boost::multiprecision::limb_type) * 8 << std::endl;
