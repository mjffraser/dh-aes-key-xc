#include "rand/random_util.hpp"
#include "logger.hpp"

#include <boost/nondet_random.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace dh {

static boost::random_device rd;

cpp_int rand_between(cpp_int& lower, cpp_int& upper) {
	Logger&	log = Logger::get();

	//define range
	boost::random::uniform_int_distribution<cpp_int> gen(lower+1, upper-1);
	cpp_int selected = gen(rd);
	log.append_to_log("[LOG] Generated value: " + selected.str());
	return selected;
}

cpp_int rand_prime(unsigned int bits) {
	//TODO
	return cpp_int();
}

std::shared_ptr<unsigned char> random_bytes(int count) {
	std::shared_ptr<unsigned char> bytes(new unsigned char[count], std::default_delete<unsigned char[]>());

	size_t i = 0;
	while (i < count) {
		unsigned char b = rd();
		bytes.get()[i] = b;
		i++;
	}

	return bytes;
}

}
