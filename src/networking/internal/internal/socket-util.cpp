#include "networking/internal/internal/socket-util.hpp"
#include "logger.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace dh {

static bool getServerAddress(ConfigParams const& params, Logger& log, sockaddr_in*& addr) {
	static bool init;
	static sockaddr_in server_addr;

	if (!init) {
		//load param values
		//these have been validated during the import process
		//default vals are used if any issue occured
		std::string  ip   = params.ip_addr;
		unsigned int port = params.port;

		server_addr.sin_family      = AF_INET;
		server_addr.sin_port        = htons(port);
		server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
		init = true;
	}

	addr = &server_addr;
	return true;
}

int createSocket(ConfigParams const& params) {
	Logger&	log	= Logger::get();

	log.log("Creating socket...");

	//create socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (params.server) {
		sockaddr_in* server_addr;
		if (getServerAddress(params, log, server_addr)) {
			int res = bind(sock, (sockaddr*)server_addr, sizeof(*server_addr));
			if (res < 0) {
				log.err("Could not bind socket.");
				return -1;
			}
		} else {
			return -1; //err message logged in getServerAddress
		}
	}
	
	return sock;
}

void closeSocket(int socket) {
	Logger&	log	= Logger::get();
	log.log("Closing socket...");
	close(socket);
}

int initConnection(int socket, ConfigParams const& params) {
	Logger& log = Logger::get();

	//server
	if (params.server) {
		listen(socket, 1); //will actually be 16, as Linux enforces a min backlog
		log.log("Listening for client.");

		int client_socket = accept(socket, nullptr, nullptr);
		log.log("Found client.");

		return client_socket; //will be negative on err
	} 

	//client
	else {
		sockaddr_in* server_addr;
		if (getServerAddress(params, log, server_addr)) {
			log.log("Attempting to connect to server.");

			int res = connect(socket, (sockaddr*)server_addr, sizeof(*server_addr));
			if (res < 0) {
				log.err("Could not connect to server.");
				return -1;
			} else {
				log.log("Connected to server.");
				return res; //will be 0 on sucess
			}
		} else {
			return -1; //err message logged in getServerAddress
		}
	}
} 

ssize_t sendMessage(int                 socket,
                    char   const* const message,
                    size_t              message_len) {
	Logger& log = Logger::get();
	log.log("Sending message...");

	uint32_t len_code = htonl(message_len+4);
	size_t total = 0;
  std::vector<char> buffer;
	std::memcpy(buffer.data(),   &len_code, 4);
	std::memcpy(buffer.data()+4, message,   message_len);
	while (total < message_len+4) {
		ssize_t sent = send(socket, buffer.data(), message_len+4, MSG_NOSIGNAL);
		if (sent < 0) {
			log.err("Could not send message.");
			return -1;
		}
		total += sent;
		log.log("Sent " + std::to_string(sent) + " bytes.");
	}

	return total;
}

ssize_t recvMessage(int socket, std::vector<char>& buffer) {
	Logger& log = Logger::get();
	log.log("Receiving message...");

	ssize_t total = 0;
	std::vector<char> temp(1024);

	ssize_t received = 1;
	uint32_t message_len = 0;
	while (received > 0) {
		received = recv(socket, temp.data(), temp.size(), 0);
		total += received;

		if (received < 0) {
			log.err("An error occured while receiving a message.");
			return -1;
		} 

		else if (received == 0) {
			if (total > 4 && total == message_len) {
				buffer.erase(buffer.begin(), buffer.begin()+4);
				return total;
			}	else {
				log.err("An error occured while receiving a message.");
				return -1;
			}

		}

		else {
			log.log("Received " + std::to_string(received) + " bytes.");
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


} //dh
