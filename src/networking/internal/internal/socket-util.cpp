#include "networking/internal/internal/socket-util.hpp"
#include "networking/internal/internal/message-formatting.hpp"
#include "networking/internal/internal/internal/socket-send-recv.hpp"
#include "aes/aes-gcm.hpp"
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

int sendMessage(int                             socket,
                std::string              const& message,
                std::optional<AESParams> const& aes_v) {
  char   const* msg_ptr = message.c_str(); 
  size_t const  msg_len = message.size();

  std::string send;

  if (aes_v) {
    //encrypt, then send

    //plaintext to encrypt
    unsigned char const* plaintext = reinterpret_cast<unsigned char const*>(msg_ptr);
    
    //ciphertext container
    std::vector<unsigned char> ciphertext;
    ciphertext.resize(msg_len); //reserve memory

    //tag container
    unsigned char tag[16];

    //encrypt message
    if (-1 == aesEncrypt(plaintext, ciphertext.data(), tag, msg_len, aes_v.value()))
      return EXIT_FAILURE; //err reported by aesEncrypt

    std::string ciphertext_hex = stoh(ciphertext.data(), msg_len);
    std::string tag_hex        = stoh(tag, sizeof(tag));
    
    send = formatMessage({
      std::make_pair(tag_hex.data(),        tag_hex.length()),
      std::make_pair(ciphertext_hex.data(), ciphertext_hex.length())
    });
  } else {
    //send unencrypted, already formatted message
    send = message;
  }

  if (-1 == sendMessage(socket, send.c_str(), send.length()))
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int recvMessage(int                             socket,
                std::string&                    message,
                std::optional<AESParams> const& aes_v) {
  Logger& log = Logger::get();
  std::vector<char> buffer;

  //receive the hex string of the message sent
  if (-1 == recvMessage(socket, buffer))
    return EXIT_FAILURE;

  if (aes_v) {
    //we have to decrypt the message

    std::string formatted_message(buffer.begin(), buffer.end());
    std::vector<std::string> messages(parseMessage(formatted_message));
    if (messages.size() != 2) {
      log.err("Message receieved does not appear to be tag||ciphertext.");
      return EXIT_FAILURE;
    }

    std::string& tag_hex        = messages[0];
    std::string& ciphertext_hex = messages[1];
    size_t const msg_len = ciphertext_hex.length()/2;

    unsigned char tag[16];
    std::vector<unsigned char> ciphertext; 
    std::vector<unsigned char> plaintext;
    ciphertext.resize(msg_len);
    plaintext.resize(msg_len);


    htos(tag_hex, tag, sizeof(tag));
    htos(ciphertext_hex, ciphertext.data(), msg_len);

    if (-1 == aesDecrypt(plaintext.data(), ciphertext.data(), tag, msg_len, aes_v.value()))
      return EXIT_FAILURE; 

    //back from unsigned char* to char*
    char* const plaintext_str = reinterpret_cast<char*>(plaintext.data());
    message = std::string(plaintext_str, msg_len);
  } else {
    //parse unencrypted, already formatted message
    message = std::string(buffer.begin(), buffer.end());
  }

  return EXIT_SUCCESS;
}


} //dh
