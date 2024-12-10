#include "networking/client.hpp"
#include "dh_params.hpp"
#include "logger.hpp"
#include "networking/internal/message_formatting.hpp"
#include "networking/internal/socket_util.hpp"
#include <ostream>

namespace dh {

int create_client(Params& params) {
	return create_socket(params);
}

void client_teardown(int socket) {
	close_socket(socket);
}

int connect_to_server(int socket, Params& params) {
	return init_connection(socket, params);
}

int recv_dh_pub(int socket, Params& params) {
	Logger& log = Logger::get();
	std::vector<char> buffer;
	ssize_t res = recv_message(socket, buffer, 5);
	if (res < 0) 
		return -1;

	//parse formatted messages into induvidual p&g hex strings
	std::string formatted_message(buffer.begin(), buffer.end());
	std::vector<std::string> messages = parse_message(formatted_message);

	if (messages.size() != 2) {
		log.append_to_log("[ERR] Message recieved does not appear to be p||g");
		return -1;
	}

	//messages[0] - p_hex
	params.p = htoi(messages[0]);
	
	//messages[1] - g_hex
	params.g = htoi(messages[1]);

	return 0;
}

int send_server_A(int client, Params& params) {
	cpp_int& A = params.A;

	std::string A_hex = itoh(A);
	return send_message(client, A_hex.c_str(), A_hex.length());
	//no need to format since we're sending a single value
}

int recv_server_B(int client, Params& params) {
	std::vector<char> buffer;
	ssize_t res = recv_message(client, buffer, 3);
	if (res < 0)
		return -1;

	std::string message(buffer.begin(), buffer.end());
	params.B = htoi(message);
	return 0;
}

int send_encrypted_message(Params& params,
													 char* message, 
													 int message_len, 
													 char* tag, 
													 char* iv) {
	return 0;
}

}
