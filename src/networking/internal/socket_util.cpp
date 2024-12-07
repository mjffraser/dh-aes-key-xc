#include "networking/internal/socket_util.hpp"
#include "dh_params.hpp"
#include "logger.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace dh {


/*
 * get_server_address
 *
 * Takes in a sockaddr_in pointer, and sets the pointer to point toward the statically
 * allocated serverAddr here. A pointer is only used so the success of the function
 * can be recorded gracefully.
 *
 * Returns:
 * - true if serverAddr can be accessed from addr
 * - false otherwise
 */
static bool get_server_address(DH_Params& params, Logger& log, sockaddr_in*& addr) {
	static bool init;
	static sockaddr_in serverAddr;

	if (!init) {
		//load param values & check initialization has occured
		std::optional<std::string>  ip   = params.get_IP();
		std::optional<unsigned int> port = params.get_port();
		if (!ip or !port) {
			//this shouldn't ever happen, but just in case
			if (params.debug())
				log.append_to_log("[ERR] Network values have not been initialized!");
			return false;
		}
		
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port.value());
		serverAddr.sin_addr.s_addr = inet_addr(ip.value().c_str());
		init = true;
	}

	addr = &serverAddr;
	return true;
}

int create_socket() {
	DH_Params& params = DH_Params::get();
	Logger&	log	= Logger::get();

	if (params.debug())
		log.append_to_log("[LOG] Creating socket.");

	//create socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (params.is_server()) {
		sockaddr_in* serverAddr;
		if (get_server_address(params, log, serverAddr)) {
			int res = bind(sock, (sockaddr*)serverAddr, sizeof(*serverAddr));
			if (res < 0) {
				if (params.debug())
					log.append_to_log("[ERR] Could not bind socket.");
				return -1;
			}
		} else {
			return -1; //err message logged in get_server_address
		}
	}
	
	return sock;
}

void close_socket(int socket) {
	DH_Params& params = DH_Params::get();
	Logger&	log	= Logger::get();

	if (params.debug())
		log.append_to_log("[LOG] Closing socket.");
	close(socket);
}

int init_connection(int socket) {
	DH_Params& params = DH_Params::get();
	Logger& log = Logger::get();

	//server
	if (params.is_server()) {
		listen(socket, 1); //will actually be 16, as Linux enforces a min backlog
		if (params.debug())
			log.append_to_log("[LOG] Listening for client.");

		int client_socket = accept(socket, nullptr, nullptr);
		if (params.debug())
			log.append_to_log("[LOG] Found client.");

		return client_socket; //will be negative on err
	} 

	//client
	else {
		sockaddr_in* serverAddr;
		if (get_server_address(params, log, serverAddr)) {
			if (params.debug())
				log.append_to_log("[LOG] Attempting to connect to server.");

			int res = connect(socket, (sockaddr*)serverAddr, sizeof(*serverAddr));
			if (res < 0) {
				if (params.debug())
					log.append_to_log("[ERR] Could not connect to server.");
				return -1;
			} else {
				if (params.debug())
					log.append_to_log("[LOG] Connected to server.");
				return res; //will be 0 on sucess
			}
		} else {
			return -1; //err message logged in get_server_address
		}
	}
}

ssize_t send_message(int socket, const unsigned char* message, size_t len) {
	DH_Params& params = DH_Params::get();
	Logger& log = Logger::get();
	if (params.debug())
		log.append_to_log("[LOG] Sending message...");

	ssize_t sent;
	sent = send(socket, message, len, MSG_NOSIGNAL);
	if (sent != len || sent < 0) {
		if (params.debug())	
			log.append_to_log("[ERR] Could not send message.");
		return -1;
	}

	if (params.debug()) 
		log.append_to_log("[LOG] Sent " + std::to_string(sent) + " bytes.");

	return sent;
}

ssize_t recv_message(int socket, std::vector<unsigned char>& buffer, int timeout) {
	DH_Params& params = DH_Params::get();
	Logger& log = Logger::get();
	if (params.debug())
		log.append_to_log("[LOG] Receiving message...");

	ssize_t total = 0;
	std::vector<unsigned char> temp(1024);
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) < 0) 
		if (params.debug())
			log.append_to_log("[WARN] Could not set recv timeout. This could cause unstable transmission, or failure altogether later on.");
	

	ssize_t received = 1;
	while (received > 0) {
		received = recv(socket, temp.data(), temp.size(), 0);
		total += received;

		if (received < 0) {
			if (params.debug())
				log.append_to_log("[ERR] An error occured while receiving a message.");
			return -1;
		}

		else if (received == 0) {
			if (params.debug())
				log.append_to_log("[LOG] End of message.");
			return total;
		}

		else {
			if (params.debug())
				log.append_to_log("[LOG] Received " + std::to_string(received) + " bytes.");
			continue;
		}
	}

	//this shouldn't be reached, as the received == 0 should return at end of message
	return total;
}

}
