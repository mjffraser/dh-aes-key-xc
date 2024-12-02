#include "logger.hpp"
#include <iostream>

namespace dh {

int Logger::flush_buffer() {
	if (!log_file.is_open())
		return 1;
	
	if (buffer_tail != 0) {
		std::string contents(buffer.begin(), buffer.begin() + buffer_tail);
		log_file << contents;
		buffer.fill('\x0');
		buffer_tail = 0;
	}

	return 0;
}

int Logger::initialize(const std::string& path) {
	if (setup)
		return 0;

	buffer.fill('\x0');
	log_file.open(path);
	if (!log_file) {
		std::cerr << "[WARN] Could not open the log file. No logging will take place." << std::endl;
		return 1;
	}

	setup = true;
	return 0;
}

int Logger::append_to_log(const std::string& message) {
	uint16_t remaining_size = BUFFER_SIZE - buffer_tail;
	
	//check size+1 since we add newline char after message to keep log clean
	if ((message.size() + 1) > remaining_size) {
		int res = flush_buffer();
		if (res == 1)
			return 1;
	} else {
		std::copy(message.begin(), message.end(), buffer.begin() + buffer_tail);
		buffer_tail += message.size();
		buffer.at(buffer_tail) = '\n';
		buffer_tail++;
		return 0;
	} 

	return append_to_log(message);
}

}
