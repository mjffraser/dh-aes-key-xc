#include "logging/logger.hpp"
#include "dhParams.hpp"


int main() {
	dh::Logger logger("log.txt");
	logger.appendToLog("hiii");
	logger.appendToLog("hello");
	return 0;
}
