#include "networking/run.hpp"
#include "logger.hpp"
#include "networking/internal/handshake.hpp"
#include "networking/internal/messaging.hpp"

#include <regex>

namespace dh {

static const std::regex user_input{R"(^[ -~]+$)"};
std::string getInput() {
  std::string message;
  std::cout << ">> ";
  std::getline(std::cin, message);
  while (!std::regex_match(message, user_input)) {
    std::cout << "Please only provide printable ASCII chars (32-126)." << std::endl;
    std::cout << ">> ";
    std::getline(std::cin, message);
  }

  return message;
}

namespace client {

int runClient(ConfigParams const& params) {
  Logger& log = Logger::get();
  int server = newSocket(params);
  if (server < 0) {
    teardown(server);
    return EXIT_FAILURE;
  }

  log.status("Connecting to server.");
  if (EXIT_FAILURE == connectToServer(server, params)) {
    teardown(server);
    return EXIT_FAILURE;
  }

  log.status("Connected.");
  std::optional<DHParams> handshake_res = dhHandshake(server);
  if (!handshake_res) {
    log.err("Failed DH handshake.");
    teardown(server);
    return EXIT_FAILURE;
  }
  DHParams dh_v = *handshake_res;

  std::optional<AESParams> keygen_res = doKeygen(dh_v);
  if (!keygen_res) {
    log.err("Failed to generate AES key.");
    teardown(server);
    return EXIT_FAILURE;
  }
  AESParams aes_v = *keygen_res;

  std::string msg = getInput();
  sendEncryptedMessage(server, msg, aes_v);

  return EXIT_SUCCESS;
}

} //client

namespace server {

int runServer(ConfigParams const& params) {
  Logger& log = Logger::get();
  int listener = newSocket(params); 
  if (listener < 0) {
    teardown(listener);
    return EXIT_FAILURE;
  }

  log.status("Waiting for client.");
  int client = acceptNewClient(listener, params);
  if (client < 0) {
    teardown(listener, client);
    return EXIT_FAILURE;
  }

  log.status("Client connected.");
  std::optional<DHParams> handshake_res = dhHandshake(client, params);
  if (!handshake_res) {
    log.err("Failed DH handshake.");
    teardown(listener, client);
    return EXIT_FAILURE;
  }
  DHParams dh_v = *handshake_res;

  std::optional<AESParams> keygen_res = doKeygen(dh_v);
  if (!keygen_res) {
    log.err("Failed to generate AES key.");
    teardown(listener, client);
    return EXIT_FAILURE;
  }
  AESParams aes_v = *keygen_res;

  std::string msg;
  recvEncryptedMessage(client, msg, aes_v);
  std::cout << msg << std::endl;

  return EXIT_SUCCESS;
}

} //server

int dhAesKXC(ConfigParams const& params) {
  if (params.server)
    return server::runServer(params);
  return client::runClient(params);
}

} //dh
