#include "networking/client.hpp"
#include "dh_params.hpp"
#include "logger.hpp"
#include "networking/internal/message_formatting.hpp"
#include "networking/internal/socket_util.hpp"

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

int recv_dh_pub(int socket, cpp_int& p, cpp_int& g) {
	Logger& log = Logger::get();
	std::vector<char> buffer;
	ssize_t res = recv_message(socket, buffer, 1);
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
	p = htoi(messages[0]);
	
	//messages[1] - g_hex
	g = htoi(messages[1]);

	return 1;
}

}
