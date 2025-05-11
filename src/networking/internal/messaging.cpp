#include "networking/internal/messaging.hpp"
#include "networking/internal/internal/socket-util.hpp"
#include "networking/internal/internal/message-formatting.hpp"
#include "logger.hpp"

#include "aes/aes-gcm.hpp"

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

int sendMessage(int                             socket,
                std::string              const& message,
                std::optional<AESParams> const& aes_v) {
  Logger& log = Logger::get();
  char   const* msg_ptr = message.c_str(); 
  size_t const  msg_len = message.size();

  std::string send;

  if (aes_v) {
    //encrypt, then send

    if (cannotEncrypt(aes_v.value())) {
      log.err("AES parameters were not initialized properly.");
      return EXIT_FAILURE;
    }

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

    if (cannotEncrypt(aes_v.value())) {
      log.err("AES parameters were not initialized properly.");
        return EXIT_FAILURE;
    }

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
