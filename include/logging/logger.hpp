#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>

namespace dh {

class Logger {
private:
	//Should be suffienciently large as to handle even the largest logging and error messages
	static const uint16_t BUFFER_SIZE = 4096;

	//buffer to store messages before writing
	std::array<char, BUFFER_SIZE> buffer;
	std::size_t bufferTail = 0; //index of first unused buffer char

	//the log file to write to
	std::ofstream	logFile;


	/*
	 * flushBuffer:
	 *	Appends buffer contents to open file.
	 *
	 *	returns -> 0 on success, 1 on error
	 */
	int flushBuffer();

public:
	/*
	 * appendToLog:
	 *	Appends a message to the buffer. May or may not flush the buffer in the process.
	 *
	 *	returns -> 0 on success, 1 on error
	 */
	int appendToLog(const std::string& message);  
		

	Logger(const std::string& logPath) {
		buffer.fill('\x0');
		logFile.open(logPath);
		if (!logFile) {
			std::cerr << "[WARN] Could not open the log file. No logging will take place." << std::endl;
		}
	}

	~Logger() {
		//flush buffer one last time in teardown.
		(void) flushBuffer();
		logFile.close();
	}

};

}
