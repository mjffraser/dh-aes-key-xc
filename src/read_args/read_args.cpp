#include "read_args/read_args.hpp"
#include "read_args/internal/general.hpp"
#include "read_args/internal/networking.hpp"

namespace dh {

int parse_args(int argc, char* argv[], Params& params) {
	parse_general_fields(argc, argv, params);
	int result = parse_networking_fields(argc, argv, params);
	return result;
}

}
