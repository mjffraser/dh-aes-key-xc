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
 * On any error defaults are used.
 */
void parse_general_fields(int argc, char* argv[], Params& params);

}
