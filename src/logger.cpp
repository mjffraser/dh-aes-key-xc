#include "logger.hpp"
#include <chrono>
#include <iostream>
#include <algorithm>

namespace dh {

int Logger::flush_buffer() {
	if (!log_file.is_open())
		return -1;
	
	if (buffer_tail != 0) {
		std::string contents(buffer.begin(), buffer.begin() + buffer_tail);
		log_file << contents;
		buffer.fill('\x0');
		buffer_tail = 0;
	}

	return 0;
}

int Logger::initialize(const std::string& path, bool debug, bool quiet, bool verbose) {
	if (setup)
		return 0;
	setup = true;
	this->debug   = debug;
	this->quiet   = quiet;
	this->verbose = verbose;

	if (!this->debug) 
		return 0; //we don't actually want to open a log if we're not in debug mode
	
	buffer.fill('\x0');
	log_file.open(path, std::ios_base::app);
	if (!log_file) {
		std::cerr << "[WARN] Could not open the log file. No logging will take place." << std::endl;
		return -1;
	}

	const auto now = std::chrono::system_clock::now();
	const std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::string time_formatted = std::ctime(&t);
	
	//drops formatting done by ctime
	time_formatted.erase(std::find(time_formatted.begin(), time_formatted.end(), '\n'), time_formatted.end());
	this->append_to_log("[" + time_formatted + "]"); //timestamp for log
	return 0;
}



int Logger::append_to_log(const std::string& message, bool printed) {
	if (((message.find("[ERR]")								!= std::string::npos)								 ) ||
		  ((message.find("Top secret message:") != std::string::npos)                ) ||
			((message.find("[WARN]")							!= std::string::npos) && !this->quiet) ||
			((message.find("[INFO]")							!= std::string::npos) && !this->quiet) ||
			( this->verbose                                                            )) {
		if (!printed) {
			std::cout << message << std::endl;
		}

	}	
		
	if (!this->debug) 
		return 0;
	
	uint16_t remaining_size = BUFFER_SIZE - buffer_tail;
	//check size+1 since we add newline char after message to keep log clean
	if ((message.size() + 1) > remaining_size) {
		int res = flush_buffer();
		if (res == -1)
			return -1;
	} else {
		std::copy(message.begin(), message.end(), buffer.begin() + buffer_tail);
		buffer_tail += message.size();
		buffer.at(buffer_tail) = '\n';
		buffer_tail++;
		return 0;
	} 

	return append_to_log(message, true);
}

}
