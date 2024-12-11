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
 * Returns:
 * - 0 on success
 * - a negative value on error.
 */
int parse_networking_fields(int argc, char* argv[], Params& params);
	
}
