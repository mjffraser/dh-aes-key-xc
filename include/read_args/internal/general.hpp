#pragma once

#include "dh_params.hpp"

namespace dh {

/*
 * parseGeneralFields
 *
 * Goes through the command line args and picks 
 * out any of the general fields recognized.
 * 
 * Sets the relevant fields in params. 
 *
 * Returns:
 * - 0 on success
 * - -1 on "error" (help menu)
 */
int parse_general_fields(int argc, char* argv[], Params& params);

}
