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

/* 
 * server protocol
 *
 * Manages the server protocol.
 * 1) Calculate p, g, b, B
 * 2) Create socket.
 * 3) Receive client.
 * 4) Send client p||g.
 * 5) Receive A from the client.
 *		5.1) Calculate DH_key 
 *		5.2) Compute AES key (SLOW)
 * 6) Send B
 * 7) Receive encrypted message.
 *		7.1) Set higher timeout on recv for msg
 *				 since client needs time to compute 
 *				 AES key.
 */

namespace dh {

int server(Params& params) {
	/* 1) Calculate p, g, b, B */
	auto[p, g] = select_public_DH_params(params);			
	//we don't technically need an err check here yet since random primes haven't been implemented.
	params.p = p;
	params.g = g;

	//b is server's a
	cpp_int a  = generate_a(params);
	if (a < 0) //generate_a reports err message
		return 1;
	params.a = a;

	cpp_int A  = generate_A(params);
	if (A < 0) //generate_A reports err message
		return 1;
	params.A = A;

	/* 1) Create socket. */
	int server = create_server(params);
	if (server < 0) {
		server_teardown(server);
		return 1;
	}

	/* 2) Receive client. */
	int client = accept_client(server, params);
	if (client < 0) {
		server_teardown(server, client);
		return 1;
	}

	/* 3) Send client p||g. */	
	if (send_p_g(client, params) < 0) {
		server_teardown(server, client);
		return 1;
	}


	server_teardown(server, client);
	return 0;
}

/* 
 * client protocol
 *
 * Manages the client protocol. 
 * 1) Create socket for client to contact server through.
 * 2) Connet to the server.
 * 3) Get p||g. 
 *		3.1) Calculate a, A.
 * 4) Send A.
 * 5) Receive B.
 *		5.1) Set higher timeout on recv for B
 *				 since server needs time to compute
 *				 AES key.
 *		5.2) Calculate DH_key.
 *		5.3) Compute AES key (SLOW).
 * 6) Send encrypted message.
 */
int client(Params& params) {
	/* 1) Create socket for client to contact server through. */	
	int client = create_client(params);
	if (client < 0) {
		client_teardown(client);
		return 1;
	}
	
	/* 2) Connect to the server. */
	if (connect_to_server(client, params) != 0) {
		client_teardown(client);
		return 1;
	}

	/* 3) Get p||g. */
	cpp_int p, g;
	if (recv_dh_pub(client, p, g) < 0) {
		client_teardown(client);
		return 1;
	}
	params.p = p;
	params.g = g;

	client_teardown(client);
	return 0;
}

} //NAMESPACE DH





int main(int argc, char* argv[]) {
	//object that will house all info needed
	dh::Params params;

	//read args, init general & network fields
	if (dh::parse_args(argc, argv, params) == 1)
		return 1;

	dh::Logger& log = dh::Logger::get();
	std::string logPath = params.log_path;
	if (!logPath.empty())
		log.initialize(logPath, params.debug);

	if (params.server) {
		return server(params);
	} 

	else {
		return client(params);	
	}
}




	// cpp_int a;
	// a = cpp_int(dh::cleanString(dh::P_6144_BITS));
	// std::cout << a.backend().size() * sizeof(boost::multiprecision::limb_type) * 8 << std::endl;
