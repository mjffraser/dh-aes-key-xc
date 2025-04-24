#pragma once

#include "dh-aes-params.hpp"

namespace dh {

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * parseNetworkingFields
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:
 * -> Reads command-line args for --server, --client, --ip, --bits, and --port
 *    args. Will exit on error if both --server and --client not set. Otherwise,
 *    the default values in config are left.
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
int parseNetworkingFields(int argc, char* argv[], ConfigParams& config);

} //dh
