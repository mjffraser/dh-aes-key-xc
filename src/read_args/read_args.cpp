#include "read_args/read_args.hpp"
#include "read_args/internal/general.hpp"
#include "read_args/internal/networking.hpp"

namespace dh {

int parse_args(int argc, char* argv[], Params& params) {
		return (parse_general_fields   (argc, argv, params) == 0) &&
					 (parse_networking_fields(argc, argv, params) == 0);
}

}
