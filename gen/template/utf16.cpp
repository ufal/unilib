#include "utf16.h"

namespace ufal {
namespace unilib {

bool utf16::valid(const char16_t* str) {
  for (; *str; str++)
    if (*str >= 0xD800 && *str < 0xDC00) {
      str++; if (*str < 0xDC00 || *str >= 0xE000) return false;
    } else if (*str >= 0xDC00 && *str < 0xE000) return false;

  return true;
}

bool utf16::valid(const char16_t* str, size_t len) {
  for (; len; str++, len--)
    if (*str >= 0xD800 && *str < 0xDC00) {
      str++; if (!--len || *str < 0xDC00 || *str >= 0xE000) return false;
    } else if (*str >= 0xDC00 && *str < 0xE000) return false;

  return true;
}

void utf16::decode(const char16_t* str, std::u32string& decoded) {
  decoded.clear();

  for (char32_t chr; (chr = decode(str)); )
    decoded.push_back(chr);
}

void utf16::decode(const char16_t* str, size_t len, std::u32string& decoded) {
  decoded.clear();

  while (len)
    decoded.push_back(decode(str, len));
}

void utf16::encode(const std::u32string& str, std::u16string& encoded) {
  encoded.clear();

  for (auto&& chr : str)
    append(encoded, chr);
}

const char16_t utf16::REPLACEMENT_CHAR;

} // namespace unilib
} // namespace ufal
