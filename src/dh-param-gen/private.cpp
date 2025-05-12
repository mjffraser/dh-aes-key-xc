#include "dh-param-gen/private.hpp"
#include "logger.hpp"

#include <boost/nondet_random.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace dh {

static boost::random_device rd;

cpp_int randBetween(cpp_int const& lower, cpp_int const& upper) {
  //define range
  boost::random::uniform_int_distribution<cpp_int> gen(lower+1, upper-1);

  //generate
  return gen(rd);
}

int privateA(DHParams& dh_v) {
  Logger& log = Logger::get();

  if (dh_v.p < 0) {
    log.err("Prime p is not initialized.");
    return EXIT_FAILURE;
  } 

  cpp_int& p = dh_v.p;

  //we choose our secret exponent a
  cpp_int& a = dh_v.a;
  a = randBetween(cpp_int(1), p-1);

  #ifdef DEBUG 
  log.log("Using a=" + a.str());
  #endif

  return EXIT_SUCCESS;
}

int publicA(DHParams& dh_v) {
  Logger& log = Logger::get();
  if (dh_v.p < 0 ||
      dh_v.g < 0 ||
      dh_v.a < 0) {
    log.err("Param p or g or a is missing.");
    return EXIT_FAILURE;
  }

  dh_v.A = powm(dh_v.g, dh_v.a, dh_v.p);
  log.log("Using A=" + dh_v.A.str());
  return EXIT_SUCCESS;
}

int dhKey(DHParams& dh_v) {
  Logger& log = Logger::get();
  if (dh_v.p < 0 ||
      dh_v.a < 0 ||
      dh_v.B < 0) {
    log.err("Param p or a or B is missing.");
    return EXIT_FAILURE;
  }

  dh_v.dh_key = powm(dh_v.B, dh_v.a, dh_v.p);
  #ifdef DEBUG
  log.log("Using dh_key=" + dh_v.dh_key.str());
  #endif

  return EXIT_SUCCESS;
}

}
