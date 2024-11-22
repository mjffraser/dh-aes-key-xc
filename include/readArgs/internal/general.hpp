#pragma once

namespace dh {

/*
 * parseGeneralFields
 *
 * Goes through the command line args and picks 
 * out any of the general fields recognized.
 * 
 * Then grabs DHParams instance and sets the fields.
 *
 * On any error defaults are used.
 */
void parseGeneralFields(int argc, char* argv[]);

}
