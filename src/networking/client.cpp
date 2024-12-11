#include "networking/client.hpp"
#include "dh_params.hpp"
#include "networking/internal/encrypt/aes_gcm.hpp"
#include "networking/internal/encrypt/keygen.hpp"
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

int recv_dh_pub(int socket, Params& params) {
	Logger& log = Logger::get();
	std::vector<char> buffer;
	ssize_t res = recv_message(socket, buffer);
	if (res < 0) 
		return -1;

	//parse formatted messages into induvidual p&g hex strings
	std::string formatted_message(buffer.begin(), buffer.end());
	std::vector<std::string> messages = parse_message(formatted_message);

	if (messages.size() != 2) {
		log.append_to_log("[ERR] Message received does not appear to be p||g");
		return -1;
	}

	//messages[0] - p_hex
	params.p = htoi(messages[0]);
	
	//messages[1] - g_hex
	params.g = htoi(messages[1]);

	return 0;
}

int send_server_A(int client, Params& params) {
	Logger& log = Logger::get();
	cpp_int& A = params.A;

	std::string A_hex = itoh(A);

	ssize_t sent = send_message(client, A_hex.c_str(), A_hex.length());
	if (sent != A_hex.length()+4) {
		log.append_to_log("[ERR] Could not send A to server.");
		return -1;
	} 

	return 0;
}

int recv_server_B(int client, Params& params) {
	std::vector<char> buffer;
	ssize_t res = recv_message(client, buffer);
	if (res < 0)
		return -1;

	std::string message(buffer.begin(), buffer.end());
	params.B = htoi(message);
	return 0;
}

void unsign(const char* from, unsigned char* to, size_t len) {
	for (size_t i = 0; i < len; ++i) 
		to[i] = (unsigned char)from[i];
}

int send_encrypted_message(int client,
													 Params& params,
													 const char* message, 
													 size_t message_len) {

	Logger& log = Logger::get();
	if (params.dh_key < 0) {
		log.append_to_log("[ERR] DH key wasn't made available to generate the AES key.");
		return -1;
	}
	log.append_to_log("[INFO] Obtaining AES-key...");
	auto [key, nonce] = aes_keygen(params.dh_key, params);
	log.append_to_log("[INFO] Done.");
	
	unsigned char aes_key   [32];
	unsigned char aes_iv    [12];
	unsigned char plaintext [message_len];
	unsigned char ciphertext[message_len];
	unsigned char tag       [16];

	//convert message to unsigned char for encryption
	unsign(message, plaintext, message_len);	

	//copy AES key and IV
	std::memcpy(aes_key, key.data(),   sizeof(aes_key));
	std::memcpy(aes_iv,  nonce.data(), sizeof(aes_iv));

	//If debug we convert AES key and IV to hex and log
	if (params.debug) {
		std::string aes_key_hex = stoh(aes_key, 32);
		std::string aes_iv_hex  = stoh(aes_iv,  12);
		log.append_to_log("[LOG] AES-KEY=" + aes_key_hex);
		log.append_to_log("[LOG] AES-IV="  + aes_iv_hex);
	}

	int res = encrypt(plaintext, message_len, aes_key, aes_iv, ciphertext, tag, params);
	if (res < 0) {
		log.append_to_log("[ERR] Encryption failed.");
		return -1;
	}	

	//convert encrypted message to a hex string representing the same data 
	std::string ciphertext_hex = stoh(ciphertext, message_len);
	std::string tag_hex = stoh(tag, sizeof(tag));
	std::string final_message = format_message({
		std::make_pair(tag_hex.data(), tag_hex.length()),
		std::make_pair(ciphertext_hex.data(), ciphertext_hex.length())
	});

	log.append_to_log("[INFO] Sending encrypted message to server.");
	
	return send_message(client, final_message.c_str(), final_message.length());
}


}
