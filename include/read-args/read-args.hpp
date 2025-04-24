#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parseArgs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Goes through all command-line args, and sets various fields in the
 *    provided config container accordingly.
 *
 * Takes:
 * -> argc:
 *    argc from main()
 * -> argv:
 *    argv from main()
 * -> config:
 *    The ConfigParams object to modify.
 *
 * Returns:
 * -> On success:
 *    EXIT_SUCCESS
 * -> On failure:
 *    EXIT_FAILURE
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */
int parseArgs(int argc, char* argv[], ConfigParams& config);

} //dh
