#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * parseArgs
 *
 * Takes the command line args supplied by the user, reads them, 
 * and fills out the general and networking sections of Params.
 *
 * Acts as a wrapper for internal functions that do the parsing.
 *
 * Returns:
 * - 0 on success
 * - 1 on critical error
 */
int parse_args(int argc, char* argv[], Params& params);
	
}
