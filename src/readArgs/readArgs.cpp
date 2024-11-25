#include "readArgs/readArgs.hpp"

#include "readArgs/internal/general.hpp"
#include "readArgs/internal/networking.hpp"
#include <iostream>

namespace dh {

int parseArgs(int argc, char* argv[]) {
	parseGeneralFields(argc, argv);
	int result = parseNetworkingFields(argc, argv);
	return result;
}

}
