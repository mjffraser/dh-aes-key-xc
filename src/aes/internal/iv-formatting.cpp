#include "aes/internal/iv-formatting.hpp"

namespace dh {

int formIV(unsigned char* dest, cpp_int const& current_iv, size_t iv_len) {
  std::vector<unsigned char> bytes;
  export_bits(current_iv, std::back_inserter(bytes), 8);

  if (bytes.size() > iv_len)
    return EXIT_FAILURE;

  std::copy(bytes.begin(), bytes.end(), dest);
  return EXIT_SUCCESS;
}

int formCppInt(cpp_int& dest, unsigned char* src, size_t src_len) {
  std::vector<unsigned char> bytes(src, src+src_len);
  import_bits(dest, bytes.begin(), bytes.end(), 8);
  return EXIT_SUCCESS;
}

}
