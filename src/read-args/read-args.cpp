#include "read-args/read-args.hpp"
#include "read-args/internal/general.hpp"
#include "read-args/internal/networking.hpp"

namespace dh {

int parseArgs(int argc, char* argv[], ConfigParams& config) {
  if (parseGeneralFields(argc, argv, config) == EXIT_FAILURE)
    return EXIT_FAILURE; //help menu
  return parseNetworkingFields(argc, argv, config); //returns EXIT_*
}

}
