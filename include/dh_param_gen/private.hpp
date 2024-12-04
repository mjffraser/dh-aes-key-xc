#pragma once

#include "dh_params.hpp" 

//Generation for private DH values done by server and client.

namespace dh {

cpp_int generate_a   ();
cpp_int generate_A   (cpp_int& p, cpp_int& g, cpp_int& a);
cpp_int generate_key (cpp_int& p, cpp_int& a, cpp_int& B);

}
