#include "logger.hpp"
#include <chrono>
#include <iostream>
#include <algorithm>

namespace dh {

using namespace std::chrono;

int Logger::flushBuffer() {
  if (!log_file.is_open()) return EXIT_FAILURE;
  if (buffer_tail != 0) {
    std::string contents(buffer.begin(), buffer.begin() + buffer_tail);
    log_file << contents;
    buffer_tail = 0;
  }

  return EXIT_SUCCESS;
}

int Logger::initialize(std::string const& path, 
                       bool               debug, 
                       bool               quiet, 
                       bool               verbose) {
  if (setup) return EXIT_SUCCESS;
  this->setup   = true;
  this->debug   = debug;
  this->quiet   = quiet;
  this->verbose = verbose;

  //we don't want to log anything
  if (!this->debug) return EXIT_SUCCESS;

  log_file.open(path, std::ios_base::app);
  if (!log_file) {
    std::cerr << "[ERR] Could not open log file." << std::endl;
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

  uint16_t remaining_size = BUFFER_SIZE - buffer_tail;
  
  //if we will overflow the buffer trying to copy to it
  if ((message.size() + 1) > remaining_size) {
    if (flushBuffer() == EXIT_FAILURE)
      return EXIT_FAILURE;
  }

  //we copy the message into the buffer
  std::copy(message.begin(), message.end(),
            buffer.begin() + buffer_tail);
  buffer_tail += message.size();
  buffer.at(buffer_tail) = '\n';
  buffer_tail++;
  return EXIT_SUCCESS;
}


int Logger::logMessage(std::string const& message) {
  return appendToLog(std::string("[LOG] " + message));
}

int Logger::errMessage(std::string const& message) {
  return appendToLog(std::string("[ERR] " + message));
}

int Logger::warnMessage(std::string const& message) {
  return appendToLog(std::string("[WARN] " + message));
}

int Logger::statusMessage(std::string const& message) {
  return appendToLog(std::string("[INFO] " + message));
}

}
