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
static bool get_server_address(Params& params, Logger& log, sockaddr_in*& addr) {
	static bool init;
	static sockaddr_in serverAddr;

	if (!init) {
		//load param values
		//these have been validated during the import process
		//default vals are used if any issue occured
		std::string  ip   = params.ip_addr;
		unsigned int port = params.port;

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
		init = true;
	}

	addr = &serverAddr;
	return true;
}

int create_socket(Params& params) {
	Logger&	log	= Logger::get();

	log.append_to_log("[LOG] Creating socket.");

	//create socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (params.server) {
		sockaddr_in* serverAddr;
		if (get_server_address(params, log, serverAddr)) {
			int res = bind(sock, (sockaddr*)serverAddr, sizeof(*serverAddr));
			if (res < 0) {
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
	Logger&	log	= Logger::get();
	log.append_to_log("[LOG] Closing socket.");
	close(socket);
}

int init_connection(int socket, Params& params) {
	Logger& log = Logger::get();

	//server
	if (params.server) {
		listen(socket, 1); //will actually be 16, as Linux enforces a min backlog
		log.append_to_log("[LOG] Listening for client.");

		int client_socket = accept(socket, nullptr, nullptr);
		log.append_to_log("[LOG] Found client.");

		return client_socket; //will be negative on err
	} 

	//client
	else {
		sockaddr_in* serverAddr;
		if (get_server_address(params, log, serverAddr)) {
			log.append_to_log("[LOG] Attempting to connect to server.");

			int res = connect(socket, (sockaddr*)serverAddr, sizeof(*serverAddr));
			if (res < 0) {
				log.append_to_log("[ERR] Could not connect to server.");
				return -1;
			} else {
				log.append_to_log("[LOG] Connected to server.");
				return res; //will be 0 on sucess
			}
		} else {
			return -1; //err message logged in get_server_address
		}
	}
}

ssize_t send_message(int socket, const char* message, size_t len) {
	Logger& log = Logger::get();
	log.append_to_log("[LOG] Sending message...");

	uint32_t len_code = htonl(len+4);
	size_t total = 0;
	char buffer[len+4];
	std::memcpy(buffer, &len_code, 4);
	std::memcpy(buffer+4, message, len);
	while (total < len+4) {
		ssize_t sent = send(socket, buffer, len+4, MSG_NOSIGNAL);
		if (sent < 0) {
			log.append_to_log("[ERR] Could not send message.");
			return -1;
		}
		total += sent;
		log.append_to_log("[LOG] Sent " + std::to_string(sent) + " bytes.");
	}

	return total;
}

ssize_t recv_message(int socket, std::vector<char>& buffer) {
	Logger& log = Logger::get();
	log.append_to_log("[LOG] Receiving message...");

	ssize_t total = 0;
	std::vector<char> temp(1024);

	ssize_t received = 1;
	uint32_t message_len = 0;
	while (received > 0) {
		received = recv(socket, temp.data(), temp.size(), 0);
		total += received;

		if (received < 0) {
			log.append_to_log("[ERR] An error occured while receiving a message.");
			return -1;
		} 

		else if (received == 0) {
			if (total > 4 && total == message_len) {
				buffer.erase(buffer.begin(), buffer.begin()+4);
				return total;
			}	else {
				log.append_to_log("[ERR] An error occured while receiving a message.");
				return -1;
			}

		}

		else {
			log.append_to_log("[LOG] Received " + std::to_string(received) + " bytes.");
			buffer.insert(buffer.end(), temp.begin(), temp.begin() + received);
			if (total > 4 && message_len == 0) {
				std::memcpy(&message_len, buffer.data(), 4);
				message_len = ntohl(message_len);
			} 
				
			if (total == message_len) {
				buffer.erase(buffer.begin(), buffer.begin()+4); //drop length bytes
				return total;
			}
			continue;
		}
	}

	//this shouldn't be reached, as the received == 0 should return at end of message
	return total;
}

}
