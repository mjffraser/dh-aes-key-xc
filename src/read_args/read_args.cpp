#include "read_args/read_args.hpp"
#include "read_args/internal/general.hpp"
#include "read_args/internal/networking.hpp"

namespace dh {

int parse_args(int argc, char* argv[]) {
	parse_general_fields(argc, argv);
	int result = parse_networking_fields(argc, argv);
	return result;
}

}
