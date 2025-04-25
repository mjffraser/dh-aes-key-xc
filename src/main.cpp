#include "dh-aes-params.hpp"
#include "dh-param-gen/private.hpp"
#include "dh-param-gen/public.hpp"
#include "logger.hpp"
#include "read-args/read-args.hpp"

int main(int argc, char* argv[]) {
  #ifdef DEBUG
  std::cerr << "\033[31m !!! YOU ARE USING A TEST BUILD OF THIS SOFTWARE. !!!\033[0m\n"; 
  #endif

  //load config, and error out if we had a problem doing so
  dh::ConfigParams config;
  if (dh::parseArgs(argc, argv, config) == EXIT_FAILURE)
    return EXIT_FAILURE;
  
  //initialize logger
  dh::Logger& log = dh::Logger::get();
  if (!config.log_path.empty())
    log.initialize(config.log_path,
                   config.debug,
                   config.quiet,
                   config.verbose);

  dh::DHParams dh_v;
  dh::selectPublicDHParams(config, dh_v);
  dh::privateA(dh_v);

  return 0;
}
