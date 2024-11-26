#pragma once

#include "num.hpp"

//Generation for private DH values done by server and client.

namespace dh {

Num generate_a   (Num& p, Num& g);
Num generate_A   (Num& p, Num& g, Num& a);
Num generate_key (Num& p, Num& a, Num& B);

}
