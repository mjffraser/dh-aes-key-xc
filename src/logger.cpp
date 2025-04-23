#include "logger.hpp"
#include <chrono>
#include <iostream>
#include <algorithm>

namespace dh {

using namespace std::chrono;

int Logger::flushBuffer() {
  if (!this->log_file.is_open())
    return EXIT_FAILURE;
  if (this->buffer_tail != 0) {
    std::string contents(this->buffer.begin(),
                         this->buffer.begin() + this->buffer_tail);
    this->log_file << contents;
    this->buffer_tail = 0;
  }
  return EXIT_SUCCESS;
}

int Logger::initialize(std::string const& path, 
                       bool               debug, 
                       bool               quiet, 
                       bool               verbose) {
  if (this->setup)
    return EXIT_SUCCESS;
  this->setup   = true;
  this->debug   = debug;
  this->quiet   = quiet;
  this->verbose = verbose;

  //we don't want to log anything
  if (!this->debug)
    return EXIT_SUCCESS;

  this->log_file.open(path, std::ios_base::app);
  if (!this->log_file) {
    std::cerr << "[ERR] Could not open log file." << std::endl;
    return EXIT_FAILURE;
  }

  auto        const now       = system_clock::now();
  std::time_t const t         = system_clock::to_time_t(now);
  std::string       formatted = std::ctime(&t);

  //drop formatting done by ctime
  formatted.erase(std::find(formatted.begin(), formatted.end(), '\n'),
                  formatted.end());

  this->appendToLog("[" + formatted + "]");
  return EXIT_SUCCESS;
}

int Logger::appendToLog(std::string const& message) {
  if (message.find("[ERR]") != std::string::npos) {
    std::cerr << message << '\n';
  } else if (((message.find("[WARN]") != std::string::npos) && !this->quiet) ||
             ((message.find("[INFO]") != std::string::npos) && !this->quiet) ||
              (this->verbose)
            ) {
    std::cout << message << '\n';
  }

  //if we're not logging anything
  if (!this->debug) return EXIT_SUCCESS;

  uint16_t remaining_size = BUFFER_SIZE - this->buffer_tail;
  
  //if we will overflow the buffer trying to copy to it
  if ((message.size() + 1) > remaining_size)
    if (this->flushBuffer() == EXIT_FAILURE)
      return EXIT_FAILURE;

  //we copy the message into the buffer
  std::copy(message.begin(), message.end(),
            this->buffer.begin() + this->buffer_tail);
  this->buffer_tail += message.size();
  this->buffer.at(this->buffer_tail) = '\n';
  this->buffer_tail++;
  return EXIT_SUCCESS;
}


int Logger::log(std::string const& message) {
  return this->appendToLog(std::string("[LOG] " + message));
}

int Logger::err(std::string const& message) {
  return this->appendToLog(std::string("[ERR] " + message));
}

int Logger::warn(std::string const& message) {
  return this->appendToLog(std::string("[WARN] " + message));
}

int Logger::status(std::string const& message) {
  return this->appendToLog(std::string("[INFO] " + message));
}

}
