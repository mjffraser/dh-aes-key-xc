#include "logging/logger.hpp"

int main() {
	dh::Logger& log = dh::Logger::get();
	log.initialize("test.txt");
	log.appendToLog("one");
	log.appendToLog("two");
	log.appendToLog("three");

	return 0;
}
