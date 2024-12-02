#pragma once

namespace dh {

/*
 * parseNetworkingFields
 *
 * Goes through the command line args and picks 
 * out any of the networking fields recognized.
 * 
 * Then grabs DHParams instance and sets the fields.
 *
 * returns 0 if all good, 1 on error
 */
int parse_networking_fields(int argc, char* argv[]);
	
}
