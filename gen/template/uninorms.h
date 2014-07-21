#ifndef UFAL_UNILIB_UNINORMS_H
#define UFAL_UNILIB_UNINORMS_H

#include <string>

namespace ufal {
namespace unilib {

class uninorms {
 public:
  static void nfc(std::u32string& str);
  static void nfd(std::u32string& str);
  static void nfkc(std::u32string& str);
  static void nfkd(std::u32string& str);

 private:
  static void compose(std::u32string& str);
  static void decompose(std::u32string& str, bool kanonical);

  static const char32_t CHARS = 0x110000;

  struct Hangul {
    // Hangul decomposition and composition
    static const char32_t SBase = 0xAC00, LBase = 0x1100, VBase = 0x1161, TBase = 0x11A7;
    static const char32_t LCount = 19, VCount = 21, TCount = 28, NCount = VCount * TCount, SCount = LCount * NCount;
  };

  static const uint8_t ccc_index[CHARS >> 8];
  static const uint8_t ccc_block[][256];

  static const uint8_t composition_index[CHARS >> 8];
  static const uint16_t composition_block[][257];
  static const char32_t composition_data[];

  static const uint8_t decomposition_index[CHARS >> 8];
  static const uint16_t decomposition_block[][257];
  static const char32_t decomposition_data[];
};

} // namespace unilib
} // namespace ufal

#endif // UFAL_UNILIB_UNINORMS_H
