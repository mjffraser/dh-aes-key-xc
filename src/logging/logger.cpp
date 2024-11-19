#include "logging/logger.hpp"

namespace dh {

int Logger::flushBuffer() {
	if (!logFile.is_open())
		return 1;
	
	if (bufferTail != 0) {
		std::string contents(buffer.begin(), buffer.begin() + bufferTail);
		logFile << contents;
		buffer.fill('\x0');
		bufferTail = 0;
	}

	return 0;
}

int Logger::appendToLog(const std::string& message) {
	uint16_t remainingSize = BUFFER_SIZE - bufferTail;
	
	//check +1 since we add newline char after message to keep log clean
	if ((message.size() + 1) > remainingSize) {
		int res = flushBuffer();
		if (res == 1)
			return 1;
	} else {
		std::copy(message.begin(), message.end(), buffer.begin() + bufferTail);
		bufferTail += message.size();
		buffer.at(bufferTail) = '\n';
		bufferTail++;
		return 0;
	} 

	return appendToLog(message);
}

}