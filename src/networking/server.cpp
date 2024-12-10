#include "networking/server.hpp"
#include "dh_params.hpp"
#include "logger.hpp"
#include "networking/internal/socket_util.hpp"
#include "networking/internal/message_formatting.hpp"

namespace dh {

int create_server(Params& params) {
	return create_socket(params);
}

void server_teardown(int server, int client) {
	if (client > 0)
		close_socket(client);
	close_socket(server);
}

int accept_client(int server, Params& params) {
	return init_connection(server, params);
}

int send_p_g(int client, Params& params) {
	Logger& log = Logger::get();
	cpp_int& p = params.p;
	cpp_int& g = params.g;
	
	if (p < 0 || g < 0) {
		log.append_to_log("[ERR] p and/or g not initialized.");
		return -1;
	}

	std::string p_hex = itoh(p);
	std::string g_hex = itoh(g);

	std::string message = format_message({
		std::make_pair(p_hex.data(), p_hex.length()),
		std::make_pair(g_hex.data(), g_hex.length())
	});
	ssize_t sent = send_message(client, message.c_str(), message.length());
	return sent;
}

int recv_client_B(int client, Params& params) {
	Logger& log = Logger::get();
	std::vector<char> buffer;
	ssize_t res = recv_message(client, buffer, 1);
	if (res < 0) {
		log.append_to_log("[ERR] An error occured while recieving A.");
		return -1;
	}

	std::string message(buffer.begin(), buffer.end());
	params.B = htoi(message);
	return 0;
}

int send_client_A(int client, Params& params) {
	Logger& log = Logger::get();
	cpp_int& A = params.A;
	if (A < 0) {
		log.append_to_log("[ERR] A not initialized.");
		return -1;
	}

	std::string A_hex = itoh(A); 
	return send_message(client, A_hex.c_str(), A_hex.length());
}

}
