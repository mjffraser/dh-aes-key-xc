#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * parseNetworkingFields
 *
 * Goes through the command line args and picks 
 * out any of the networking fields recognized.
 * 
 * Sets the relevant fields in params. 
 *
 * returns 0 if all good, -1 on error due to critical
 * missing info.
 */
int parse_networking_fields(int argc, char* argv[], Params& params);
	
}
