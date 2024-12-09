#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>

namespace dh {

class Logger {
private:
	bool setup = false;
	bool debug = false;

	//Should be suffienciently large as to handle even the largest logging and error messages
	static const uint16_t BUFFER_SIZE = 4096;

	//buffer to store messages before writing
	std::array<char, BUFFER_SIZE> buffer;
	std::size_t buffer_tail = 0; //index of first unused buffer char

	//the log file to write to
	std::ofstream log_file;

	/*
	 * flushBuffer:
	 *	Appends buffer contents to open file.
	 *
	 *	returns -> 0 on success, 1 on error
	 */
	int flush_buffer();

	//enforce single instance
	//CONSTRUCTOR
	Logger() {}

	//DESTRUCTOR
	~Logger() {
		(void) flush_buffer();
	}

public:
	/*
	 * initialize:
	 *	Opens log file, prepares buffer.
	 *
	 * returns -> 0 on success, 1 on error.
	 */
	int initialize(const std::string& path, bool debug);

	/*
	 * appendToLog:
	 *	Appends a message to the buffer. May or may not flush the buffer in the process.
	 *	Errors if Logger not initialized properly.
	 *
	 *	returns -> 0 on success, 1 on error
	 */
	int append_to_log(const std::string& message);  
		
	//GET INSTANCE HERE
	static Logger& get() {
		static Logger instance;
		return instance;
	}

	//COPY & MOVE
	Logger(Logger const&)					= delete;
	void operator=(Logger const&)	= delete;

};

}
