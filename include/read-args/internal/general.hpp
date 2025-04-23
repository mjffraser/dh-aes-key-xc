#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parseGeneralFields
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Reads command-line args for --verbose, --quiet, --debug, --log, and --help
 *    args. Will exit as an error if --help is given to signal to exit. This
 *    function prints the help menu directly, circumventing the logger.
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
int parseGeneralFields(int argc, char* argv[], ConfigParams& config);

} //dh
