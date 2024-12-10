#include "networking/server.hpp"
#include "dh_params.hpp"
#include "logger.hpp"
#include "networking/internal/encrypt/aes_gcm.hpp"
#include "networking/internal/encrypt/keygen.hpp"
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
	Logger& log = Logger::get();
	cpp_int& p = params.p;
	cpp_int& g = params.g;
	
	if (p < 0 || g < 0) {
		log.append_to_log("[ERR] p and/or g not initialized.");
		return -1;
	}

	std::string p_hex = itoh(p);
	std::string g_hex = itoh(g);

	std::string message = format_message({
		std::make_pair(p_hex.data(), p_hex.length()),
		std::make_pair(g_hex.data(), g_hex.length())
	});
	ssize_t sent = send_message(client, message.c_str(), message.length());
	return sent;
}

int recv_client_B(int client, Params& params) {
	Logger& log = Logger::get();
	std::vector<char> buffer;
	ssize_t res = recv_message(client, buffer, 1);
	if (res < 0) {
		log.append_to_log("[ERR] An error occured while recieving A.");
		return -1;
	}

	std::string message(buffer.begin(), buffer.end());
	params.B = htoi(message);
	return 0;
}

int send_client_A(int client, Params& params) {
	Logger& log = Logger::get();
	cpp_int& A = params.A;
	if (A < 0) {
		log.append_to_log("[ERR] A not initialized.");
		return -1;
	}

	std::string A_hex = itoh(A); 
	return send_message(client, A_hex.c_str(), A_hex.length());
}

void sign(const unsigned char* from, std::vector<char>& to, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		to.push_back((char)from[i]);
	} 
}

int recv_encrypted_message(int client,
													 Params& params,
													 std::vector<char>& dest) {

	Logger& log = Logger::get();
	if (params.dh_key < 0) {
		log.append_to_log("[ERR] DH key wasn't made available to generate the AES key.");
		return -1;
	}

	auto [key, nonce] = aes_keygen(params.dh_key, params);
	unsigned char			aes_key[32];
	unsigned char			aes_iv [12];
	std::vector<char> buff;
	
	//copy AES key and IV
	std::memcpy(aes_key, key.data(),   sizeof(aes_key));
	std::memcpy(aes_iv,  nonce.data(), sizeof(aes_iv));

	ssize_t res = recv_message(client, buff, 3);
	if (res < 0)
		return -1;

	std::string formatted_message(buff.begin(), buff.end());
	std::vector<std::string> messages(parse_message(formatted_message));
	if (messages.size() != 2) {
		log.append_to_log("[ERR] Message recieved does not appear to be tag||ciphertext");
		return -1;
	}

	std::string tag_hex = messages[0];
	std::string ciphertext_hex = messages[1];

	unsigned char	tag[16];
	unsigned char ciphertext[ciphertext_hex.length()/2];
	unsigned char plaintext[ciphertext_hex.length()/2];

	//copy ciphertext and tag
	htos(tag_hex, tag, 16);
	htos(ciphertext_hex, ciphertext, ciphertext_hex.length()/2);

	res = decrypt(ciphertext, sizeof(ciphertext), tag, aes_key, aes_iv, plaintext, params);
	if (res < 0) {
		log.append_to_log("[ERR] Failed to decrypt message.");
		return -1;
	}

	sign(plaintext, dest, sizeof(plaintext));

	return 0;
}

}
