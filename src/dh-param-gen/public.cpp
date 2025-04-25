#include "dh-param-gen/public.hpp"
#include "dh-param-gen/internal/rfc-3526-primes.hpp"

namespace dh {

int selectPublicDHParams(ConfigParams const& config,
                         DHParams&           dh_v) {
  dh_v.p = cpp_int(vettedP(config.bits));
  dh_v.g = cpp_int(vettedG());
  if (config.bits != (dh_v.p.backend().size() * sizeof(boost::multiprecision::limb_type) * 8)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

} //dh
