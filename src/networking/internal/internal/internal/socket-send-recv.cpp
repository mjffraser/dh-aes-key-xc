#include "networking/internal/internal/internal/socket-send-recv.hpp"
#include "logger.hpp"

#include <cstring>
#include <iostream>
#include <netinet/in.h>

namespace dh {

ssize_t sendMessage(int                 socket,
                    char   const* const message,
                    size_t              message_len) {
	Logger& log = Logger::get();
	log.log("Sending message...");

	uint32_t len_code = htonl(message_len);
	size_t total = 0;
  std::vector<char> buffer;
  buffer.reserve(message_len+4);
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

	std::vector<char> len_bytes;
  len_bytes.resize(4);

  ssize_t recvd = recv(socket, len_bytes.data(), 4, 0);
  if (recvd != 4) {
    log.err("Did not receive a valid message len.");
    return -1;
  }

  uint32_t len_code; std::memcpy(&len_code, len_bytes.data(), 4); 
  ssize_t  msg_len = ntohl(len_code);

  recvd = 0;
  buffer.clear();
  buffer.resize(msg_len);
  while (recvd < msg_len) {
    auto chunk = recv(socket, buffer.data(), msg_len-recvd, 0);
    if (chunk > 0)
      recvd += chunk;
    
    if (chunk <= 0)
      return recvd;
  }
	
	return recvd;
}

} //dh
