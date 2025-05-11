#include "networking/internal/internal/internal/socket-send-recv.hpp"
#include "logger.hpp"

#include <cstring>
#include <netinet/in.h>

namespace dh {

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
