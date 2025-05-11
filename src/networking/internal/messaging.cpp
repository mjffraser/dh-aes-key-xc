#include "networking/internal/messaging.hpp"
#include "networking/internal/internal/socket-util.hpp"
#include "logger.hpp"

namespace dh {

bool cannotEncrypt(AESParams const& aes_v) {
  for (int i = 0; i < 32; ++i) {
    if (aes_v.aes_key[i] != 0)
      break;
    if (i == 32-1)
      return true; //key never initialized
  }

  if (aes_v.aes_iv < 0)
    return true;

  return false;
}

int sendEncryptedMessage(int                socket, 
                         std::string const& message, 
                         AESParams   const& aes_v) {
  if (cannotEncrypt(aes_v)) {
    Logger& log = Logger::get();
    log.err("AES parameters were not initialized properly. Cannot encrypt.");
    return EXIT_FAILURE;
  }

  return sendMessage(socket, message, aes_v);
}

int recvEncryptedMessage(int                 socket,
                         std::string&        message,
                         AESParams    const& aes_v) {
  if (cannotEncrypt(aes_v)) {
    Logger& log = Logger::get();
    log.err("AES parameters were not initialized properly. Cannot decrypt.");
    return EXIT_FAILURE;
  }

  return recvMessage(socket, message, aes_v);
}

} //dh
