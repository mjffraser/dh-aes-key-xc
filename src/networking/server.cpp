#include "networking/server.hpp"
#include "dh_params.hpp"
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
	std::string p_hex = itoh(params.p);
	std::string g_hex = itoh(params.g);

	std::string message = format_message({
		std::make_pair(p_hex.data(), p_hex.length()),
		std::make_pair(g_hex.data(), g_hex.length())
	});
	return send_message(client, message.c_str(), message.length());
}

}
