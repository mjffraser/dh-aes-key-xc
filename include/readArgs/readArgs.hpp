#pragma once

namespace dh {

/*
 * parseArgs
 *
 * Takes the command line args supplied by the user, reads them, 
 * and fills out the general and networking sections of DHParams.
 *
 * Acts as a wrapper for internal functions that do the parsing.
 *
 * returns 0 if all good, 1 on error
 */
int parseArgs(int argc, char* argv[]);

}
