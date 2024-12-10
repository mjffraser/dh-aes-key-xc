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

#include <regex>


namespace dh {

/* 
 * server protocol
 *
 * Manages the server protocol.
 * 1) Calculate p, g, b, B
 * 2) Create socket.
 * 3) Receive client.
 * 4) Send client p||g.
 * 5) Receive B from the client.
 *		5.1) Calculate DH_key 
 *		5.2) Compute AES key (SLOW)
 * 6) Send A
 * 7) Receive encrypted message.
 *		7.1) Set higher timeout on recv for msg
 *				 since client needs time to compute 
 *				 AES key.
 */
int server(Params& params) {
	Logger& log = Logger::get();
	/* 1) Calculate p, g, b, B */
	auto[p, g] = select_public_DH_params(params);			
	params.p = p;
	params.g = g;

	cpp_int a  = generate_a(params);
	if (a < 0) //generate_a reports err message
		return 1;
	params.a = a;

	cpp_int A  = generate_A(params);
	if (A < 0) //generate_A reports err message
		return 1;
	params.A = A;

	/* 2) Create socket. */
	int server = create_server(params);
	if (server < 0) {
		server_teardown(server);
		return 1;
	}

	log.append_to_log("[INFO] Waiting for client.");

	/* 3) Receive client. */
	int client = accept_client(server, params);
	if (client < 0) {
		server_teardown(server, client);
		return 1;
	}

	log.append_to_log("[INFO] Accepted client connection.");

	/* 4) Send client p||g. */	
	if (send_p_g(client, params) < 0) {
		server_teardown(server, client);
		return 1;
	}

	log.append_to_log("[INFO] Sent client p||g.");

	/* 5) Receive A from the client. */
	if (recv_client_B(client, params) < 0) {
		server_teardown(server, client);
		return 1;
	}

	log.append_to_log("[INFO] Calculating DH key.");

	/* 5.1) Calculate DH key */
	if ((params.dh_key = generate_key(params)) < 0) {
		server_teardown(server, client);
		return 1;
	}

	/* 6) Send B */
	if (send_client_A(client, params) < 0) {
		server_teardown(server, client);
		return 1;
	}

	std::vector<char> decrypted_message;
	int res = recv_encrypted_message(client, params, decrypted_message);
	std::cout << "Top secret message: ";
	for (auto& c : decrypted_message) {
		std::cout << c;
	} std::cout << std::endl;


	server_teardown(server, client);
	return 0;
}

/*
 *
 */
static const std::regex user_input{R"(^[ -~]+$)"};


/* 
 * client protocol
 *
 * Manages the client protocol. 
 * 1) Create socket for client to contact server through.
 *		1.1) Before connecting, get user input.
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
	Logger& log = Logger::get();
	/* 1) Create socket for client to contact server through. */	
	int client = create_client(params);
	if (client < 0) {
		client_teardown(client);
		return 1;
	}

	/* 1.1) Before connecting, get user input */
	std::string message;
	std::cout << "Please enter your top secret message to be delivered: ";
	std::getline(std::cin, message);	

	std::smatch match;	

	while (!std::regex_match(message, user_input)) {
		std::cout << "Please only provide printable ASCII chars (32-126)." << std::endl;
		std::cout << "Please enter your top secret message to be delivered: ";
		std::getline(std::cin, message);
	}

	log.append_to_log("[INFO] Connecting to server.");

	/* 2) Connect to the server. */
	if (connect_to_server(client, params) != 0) {
		client_teardown(client);
		return 1;
	}

	log.append_to_log("[INFO] Connected.");
	log.append_to_log("[INFO] Receiving p||g.");

	/* 3) Get p||g. */
	cpp_int p, g;
	if (recv_dh_pub(client, params) < 0) {
		client_teardown(client);
		return 1;
	}

	log.append_to_log("[INFO] Calculating private values.");


	//a
	if ((params.a = generate_a(params)) < 0) {
		client_teardown(client);
		return 1;
	}

	//A
	if ((params.A = generate_A(params)) < 0) {
		client_teardown(client);
		return 1;
	}

	/* 4) Send A */
	if (send_server_A(client, params) < 0) {
		client_teardown(client);
		return 1;
	}

	log.append_to_log("[INFO] Receiving B...");

	/* 5) Receive B */
	if (recv_server_B(client, params) < 0) {
		client_teardown(client);
		return 1;
	}

	log.append_to_log("[INFO] Calculating DH key.");

	//dh key
	if ((params.dh_key = generate_key(params)) < 0) {
		client_teardown(client);
		return 1;
	}	

	/* 6)  Send encrypted message. */
	if (send_encrypted_message(client, params, message.c_str(), message.length()) < 0 ) {
		client_teardown(client);
		return 1;
	}

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

	//initialize the logger with debug flag and log path
	dh::Logger& log = dh::Logger::get();
	std::string logPath = params.log_path;
	if (!logPath.empty())
		log.initialize(logPath, params.debug, params.quiet, params.verbose);

	try {
		if (params.server) 
			return server(params);
		else 
			return client(params);	
	} catch (...) { //anything happens we want to be able to see the log
		return 1;
	}
}

