#include "networking/internal/handshake.hpp"
#include "dh-param-gen/private.hpp"
#include "dh-param-gen/public.hpp"
#include "networking/internal/internal/message-formatting.hpp"
#include "networking/internal/internal/socket-util.hpp"
#include "networking/internal/messaging.hpp"
#include "aes/aes-keygen.hpp"
#include "logger.hpp"

namespace dh {

int newSocket(ConfigParams const& params) {
  return createSocket(params);
}

std::optional<AESParams> doKeygen(DHParams const& dh_v) {
  Logger& log = Logger::get();

  if (dh_v.dh_key < 0) {
    log.err("DH key was not derived properly.");
    return std::nullopt;
  }

  AESParams aes_v;
  if (EXIT_FAILURE == aesKeygen(dh_v, aes_v))
    return std::nullopt;
  return aes_v;
}

namespace client {

void teardown(int server) {
  closeSocket(server);
}

int connectToServer(int serv_sock, ConfigParams const& params) {
  return initConnection(serv_sock, params);
}

std::optional<DHParams> dhHandshake(int server) {
  Logger& log = Logger::get(); 
  DHParams dh_v;
  std::string last_received;

  //Receives p & g from the server
  if (EXIT_FAILURE == recvMessage(server, last_received)) {
    log.err("An error occured receiving prime p & generator g.");
    return std::nullopt;
  }

  std::vector<std::string> messages = parseMessage(last_received);
  if (messages.size() != 2) {
    log.err("Received message does not appear to be p||g");
    return std::nullopt;
  }

  dh_v.p = htoi(messages[0]); //p_hex
  dh_v.g = htoi(messages[1]); //g_hex

  //Picks a, sends A
  if (EXIT_FAILURE == privateA(dh_v)) {
    log.err("Failed while picking private exponent.");
    return std::nullopt;
  }

  if (EXIT_FAILURE == publicA(dh_v)) {
    log.err("Failed while calculating public A for server.");
    return std::nullopt;
  }

  cpp_int& A = dh_v.A;
  std::string A_hex = itoh(A);
  if (EXIT_FAILURE == sendMessage(server, A_hex)) {
    log.err("Failed while sending A to server.");
    return std::nullopt;
  }

  //Receives B
  if (EXIT_FAILURE == recvMessage(server, last_received)) {
    log.err("An error occured receiving B from the server.");
    return std::nullopt;
  }

  dh_v.B = htoi(last_received);

  //Calculates dh_key
  if (EXIT_FAILURE == dhKey(dh_v)) {
    log.err("Failed while calculating shared key.");
    return std::nullopt;
  }

  return dh_v;
}

} //client

namespace server {

void teardown(int my_sock, int client) {
  if (client >= 0)
    closeSocket(client);
  closeSocket(my_sock);
}

int acceptNewClient(int my_sock, ConfigParams const& params) {
  return initConnection(my_sock, params);
}

std::optional<DHParams> dhHandshake(int client, ConfigParams const& params) {
  Logger& log = Logger::get(); 
  DHParams dh_v;
  std::string last_received;

  //Picks a prime p and generator g with the config in params
  if (EXIT_FAILURE == selectPublicDHParams(params, dh_v)) {
    log.err("Failed while selecting p&g.");
    return std::nullopt;
  }

  //Sends p & g to client
  cpp_int& p = dh_v.p;
  cpp_int& g = dh_v.g;
  std::string p_hex = itoh(p);
  std::string g_hex = itoh(g);
  std::string message = formatMessage({
    std::make_pair(p_hex.data(), p_hex.length()),
    std::make_pair(g_hex.data(), g_hex.length())
  });

  if (EXIT_FAILURE == sendMessage(client, message)) {
    log.err("Failed while sending p&g to client.");
    return std::nullopt;
  }
  
  //Receives A
  if (EXIT_FAILURE == recvMessage(client, last_received)) {
    log.err("Failed while receiving A from the client.");
    return std::nullopt;
  }

  //we use B inside the container to signify other side's public val
  dh_v.B = htoi(last_received);

  //Picks b, sends B
  if (EXIT_FAILURE == privateA(dh_v)) {
    log.err("Failed while picking private exponent.");
    return std::nullopt;
  }

  if (EXIT_FAILURE == publicA(dh_v)) {
    log.err("Failed while calculating public B for client.");
    return std::nullopt;
  }

  cpp_int& B = dh_v.A; //server A is B from client's perspective
  message = itoh(B);
  if (EXIT_FAILURE == sendMessage(client, message)) {
    log.err("Failed while sending B to client.");
    return std::nullopt;
  }

  //Calculate dh_key
  if (EXIT_FAILURE == dhKey(dh_v)) {
    log.err("Failed while calculating shared key.");
    return std::nullopt;
  }

  return dh_v;
}

} //server

} //dh
