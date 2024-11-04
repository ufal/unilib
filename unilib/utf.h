// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014-2024 Institute of Formal and Applied Linguistics, Faculty
// of Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// UniLib version: 4.1.2-dev
// Unicode version: 16.0.0

#pragma once

#include <iterator>
#include <string>
#include <string_view>

namespace unilib {

// The `utf` class converts among UTF-8, UTF-16, and UTF-32 encodings.
// - UTF-8 represents code units as `char` and strings as
//   `const char*`, `std::string`, and `std::string_view`;
// - UTF-16 represents code units as `char16_t` and strings as
//   `const char16_t*`, `std::u16string`, and `std::u16string_view`;
// - UTF-32 represents code units as `char32_t` and strings as
//   `const char32_t*`, `std::u32string`, and `std::u32string_view`.
// The C-style "pointer-to-first-character" strings are ended by a `\0` code point;
// the STL strings behave as buffers of a given length and can contain the `\0`.
class utf {
 public:
  // Validity
  inline static bool valid(const char* str);
  inline static bool valid(std::string_view str);

  inline static bool valid(const char16_t* str);
  inline static bool valid(std::u16string_view str);

  inline static bool valid(const char32_t* str);
  inline static bool valid(std::u32string_view str);

  // First code point decoding
  inline static char32_t first(const char* str);
  inline static char32_t first(std::string_view str);

  inline static char32_t first(const char16_t* str);
  inline static char32_t first(std::u16string_view str);

  // Decoding and moving past a first code point
  inline static char32_t decode(const char*& str);
  inline static char32_t decode(std::string_view& str);

  inline static char32_t decode(const char16_t*& str);
  inline static char32_t decode(std::u16string_view& str);

  // Decoding of a whole string
  inline static void decode(const char* str, std::u32string& decoded);
  inline static void decode(std::string_view str, std::u32string& decoded);

  inline static void decode(const char16_t* str, std::u32string& decoded);
  inline static void decode(std::u16string_view str, std::u32string& decoded);

  // Iterator decoding
  template<class Char>
  class string_decoder {
   public:
    class iterator;
    inline iterator begin();
    inline iterator end();
   private:
    inline string_decoder(const Char* str);
    const Char* str;
    friend class utf;
  };
  inline static string_decoder<char> decoder(const char* str);
  inline static string_decoder<char16_t> decoder(const char16_t* str);

  template<class Char>
  class buffer_decoder {
   public:
    class iterator;
    inline iterator begin();
    inline iterator end();
   private:
    inline buffer_decoder(std::basic_string_view<Char> str);
    std::basic_string_view<Char> str;
    friend class utf;
  };
  inline static buffer_decoder<char> decoder(std::string_view str);
  inline static buffer_decoder<char16_t> decoder(std::u16string_view str);

  // Appending a single code point
  inline static void append(char*& str, char32_t chr);
  inline static void append(std::string& str, char32_t chr);

  inline static void append(char16_t*& str, char32_t chr);
  inline static void append(std::u16string& str, char32_t chr);

  // Encoding a whole string
  static void encode(const char32_t* str, std::string& encoded);
  static void encode(std::u32string_view str, std::string& encoded);

  static void encode(const char32_t* str, std::u16string& encoded);
  static void encode(std::u32string_view str, std::u16string& encoded);

 private:
  // The REPLACEMENT_CHAR used to represent invalid code points.
  inline static const char REPLACEMENT_CHAR = '?';
};

//
// Definitions
//

// Validity, UTF-8
bool utf::valid(const char* str) {
  for (; *str; str++)
    if (((unsigned char)*str) >= 0x80) {
      if (((unsigned char)*str) < 0xC0) return false;
      else if (((unsigned char)*str) < 0xE0) {
        str++; if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return false;
      } else if (((unsigned char)*str) < 0xF0) {
        str++; if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return false;
        str++; if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return false;
      } else if (((unsigned char)*str) < 0xF8) {
        str++; if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return false;
        str++; if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return false;
        str++; if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return false;
      } else return false;
    }
  return true;
}

bool utf::valid(std::string_view str) {
  for (; !str.empty(); str.remove_prefix(1))
    if (((unsigned char)str.front()) >= 0x80) {
      if (((unsigned char)str.front()) < 0xC0) return false;
      else if (((unsigned char)str.front()) < 0xE0) {
        str.remove_prefix(1); if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return false;
      } else if (((unsigned char)str.front()) < 0xF0) {
        str.remove_prefix(1); if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return false;
        str.remove_prefix(1); if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return false;
      } else if (((unsigned char)str.front()) < 0xF8) {
        str.remove_prefix(1); if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return false;
        str.remove_prefix(1); if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return false;
        str.remove_prefix(1); if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return false;
      } else return false;
    }
  return true;
}

// Validity, UTF-16
bool utf::valid(const char16_t* str) {
  for (; *str; str++)
    if (*str >= 0xD800 && *str < 0xDC00) {
      str++; if (*str < 0xDC00 || *str >= 0xE000) return false;
    } else if (*str >= 0xDC00 && *str < 0xE000) return false;

  return true;
}

bool utf::valid(std::u16string_view str) {
  for (; !str.empty(); str.remove_prefix(1))
    if (str.front() >= 0xD800 && str.front() < 0xDC00) {
      str.remove_prefix(1); if (str.empty() || str.front() < 0xDC00 || str.front() >= 0xE000) return false;
    } else if (str.front() >= 0xDC00 && str.front() < 0xE000) return false;

  return true;
}

// Validity, UTF-32
bool utf::valid(const char32_t* str) {
  for (; *str; str++)
    if (*str >= 0x110000 || (*str >= 0xD800 && *str < 0xE000)) return false;
  return true;
}

bool utf::valid(std::u32string_view str) {
  for (; !str.empty(); str.remove_prefix(1))
    if (str.front() >= 0x110000 || (str.front() >= 0xD800 && str.front() < 0xE000)) return false;
  return true;
}

// First code point decoding
char32_t utf::first(const char* str) {
  return decode(str);
}

char32_t utf::first(std::string_view str) {
  return decode(str);
}

char32_t utf::first(const char16_t* str) {
  return decode(str);
}

char32_t utf::first(std::u16string_view str) {
  return decode(str);
}

// Decoding and moving past a first code point, UTF-8
char32_t utf::decode(const char*& str) {
  if (((unsigned char)*str) < 0x80) {
    return (unsigned char)*str++;
  } else if (((unsigned char)*str) < 0xC0) {
    ++str; return REPLACEMENT_CHAR;
  } else if (((unsigned char)*str) < 0xE0) {
    char32_t res = (((unsigned char)*str++) & 0x1F) << 6;
    if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return REPLACEMENT_CHAR;
    return res + (((unsigned char)*str++) & 0x3F);
  } else if (((unsigned char)*str) < 0xF0) {
    char32_t res = (((unsigned char)*str++) & 0x0F) << 12;
    if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return REPLACEMENT_CHAR;
    res += (((unsigned char)*str++) & 0x3F) << 6;
    if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return REPLACEMENT_CHAR;
    return res + (((unsigned char)*str++) & 0x3F);
  } else if (((unsigned char)*str) < 0xF8) {
    char32_t res = (((unsigned char)*str++) & 0x07) << 18;
    if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return REPLACEMENT_CHAR;
    res += (((unsigned char)*str++) & 0x3F) << 12;
    if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return REPLACEMENT_CHAR;
    res += (((unsigned char)*str++) & 0x3F) << 6;
    if (((unsigned char)*str) < 0x80 || ((unsigned char)*str) >= 0xC0) return REPLACEMENT_CHAR;
    return res + (((unsigned char)*str++) & 0x3F);
  } else {
    ++str; return REPLACEMENT_CHAR;
  }
}

char32_t utf::decode(std::string_view& str) {
  if (str.empty()) return 0;
  char32_t res;
  if (((unsigned char)str.front()) < 0x80) {
    res = (unsigned char)str.front(); str.remove_prefix(1); return res;
  } else if (((unsigned char)str.front()) < 0xC0) {
    str.remove_prefix(1); return REPLACEMENT_CHAR;
  } else if (((unsigned char)str.front()) < 0xE0) {
    res = (((unsigned char)str.front()) & 0x1F) << 6; str.remove_prefix(1);
    if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return REPLACEMENT_CHAR;
    res += ((unsigned char)str.front()) & 0x3F; str.remove_prefix(1); return res;
  } else if (((unsigned char)str.front()) < 0xF0) {
    res = (((unsigned char)str.front()) & 0x0F) << 12; str.remove_prefix(1);
    if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return REPLACEMENT_CHAR;
    res += (((unsigned char)str.front()) & 0x3F) << 6; str.remove_prefix(1);
    if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return REPLACEMENT_CHAR;
    res += ((unsigned char)str.front()) & 0x3F; str.remove_prefix(1); return res;
  } else if (((unsigned char)str.front()) < 0xF8) {
    char32_t res = (((unsigned char)str.front()) & 0x07) << 18; str.remove_prefix(1);
    if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return REPLACEMENT_CHAR;
    res += (((unsigned char)str.front()) & 0x3F) << 12; str.remove_prefix(1);
    if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return REPLACEMENT_CHAR;
    res += (((unsigned char)str.front()) & 0x3F) << 6; str.remove_prefix(1);
    if (str.empty() || ((unsigned char)str.front()) < 0x80 || ((unsigned char)str.front()) >= 0xC0) return REPLACEMENT_CHAR;
    res += ((unsigned char)str.front()) & 0x3F; str.remove_prefix(1); return res;
  } else {
      str.remove_prefix(1); return REPLACEMENT_CHAR;
  }
}

// Decoding and moving past a first code point, UTF-16
char32_t utf::decode(const char16_t*& str) {
  if (*str < 0xD800 || *str >= 0xE000) return *str++;
  if (*str >= 0xDC00) {++str; return REPLACEMENT_CHAR;}
  char32_t res = 0x10000 + ((*str++ - 0xD800) << 10);
  if (*str < 0xDC00 || *str >= 0xE000) return REPLACEMENT_CHAR;
  return res + (*str++ - 0xDC00);
}

char32_t utf::decode(std::u16string_view& str) {
  if (str.empty()) return 0;
  char32_t res;
  if (str.front() < 0xD800 || str.front() >= 0xE000) {res = str.front(); str.remove_prefix(1); return res;}
  if (str.front() >= 0xDC00) {str.remove_prefix(1); return REPLACEMENT_CHAR;}
  res = 0x10000 + ((str.front() - 0xD800) << 10); str.remove_prefix(1);
  if (str.empty() || str.front() < 0xDC00 || str.front() >= 0xE000) return REPLACEMENT_CHAR;
  res += str.front() - 0xDC00; str.remove_prefix(1);
  return res;
}

// Decoding of a whole string
void utf::decode(const char* str, std::u32string& decoded) {
  decoded.clear();
  for (char32_t chr; (chr = decode(str)); )
    decoded.push_back(chr);
}

void utf::decode(std::string_view str, std::u32string& decoded) {
  decoded.clear();
  while (!str.empty())
    decoded.push_back(decode(str));
}

void utf::decode(const char16_t* str, std::u32string& decoded) {
  decoded.clear();
  for (char32_t chr; (chr = decode(str)); )
    decoded.push_back(chr);
}

void utf::decode(std::u16string_view str, std::u32string& decoded) {
  decoded.clear();
  while (!str.empty())
    decoded.push_back(decode(str));
}

// Iterator decoding, C-string
template<class Char>
class utf::string_decoder<Char>::iterator {
 public:
  using iterator_category = std::input_iterator_tag;
  using value_type = char32_t;
  using difference_type = ptrdiff_t;
  using pointer = char32_t*;
  using reference = char32_t&;
  iterator(const Char* str) : codepoint(0), next(str) { operator++(); }
  iterator(const iterator& it) : codepoint(it.codepoint), next(it.next) {}
  iterator& operator++() { if (next) { codepoint = decode(next); if (!codepoint) next = nullptr; } return *this; }
  iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const iterator& other) const { return next == other.next; }
  bool operator!=(const iterator& other) const { return next != other.next; }
  char32_t operator*() const { return codepoint; }
 private:
  char32_t codepoint;
  const Char* next;
};

template<class Char>
utf::string_decoder<Char>::string_decoder(const Char* str) : str(str) {}

template<class Char>
typename utf::string_decoder<Char>::iterator utf::string_decoder<Char>::begin() {
  return iterator(str);
}

template<class Char>
typename utf::string_decoder<Char>::iterator utf::string_decoder<Char>::end() {
  return iterator(nullptr);
}

utf::string_decoder<char> utf::decoder(const char* str) {
  return string_decoder(str);
}

utf::string_decoder<char16_t> utf::decoder(const char16_t* str) {
  return string_decoder(str);
}

// Iterator decoding, string_view
template<class Char>
class utf::buffer_decoder<Char>::iterator {
 public:
  using iterator_category = std::input_iterator_tag;
  using value_type = char32_t;
  using difference_type = ptrdiff_t;
  using pointer = char32_t*;
  using reference = char32_t&;
  iterator(std::basic_string_view<Char> str) : codepoint(0), next(str) { operator++(); }
  iterator(const iterator& it) : codepoint(it.codepoint), next(it.next) {}
  iterator& operator++() { if (!next.empty()) codepoint = decode(next); else next = std::basic_string_view<Char>(); return *this; }
  iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const iterator& other) const { return next.data() == other.next.data(); }
  bool operator!=(const iterator& other) const { return next.data() != other.next.data(); }
  char32_t operator*() const { return codepoint; }
 private:
  char32_t codepoint;
  std::basic_string_view<Char> next;
};

template<class Char>
utf::buffer_decoder<Char>::buffer_decoder(std::basic_string_view<Char> str) : str(str) {}

template<class Char>
typename utf::buffer_decoder<Char>::iterator utf::buffer_decoder<Char>::begin() {
  return iterator(str);
}

template<class Char>
typename utf::buffer_decoder<Char>::iterator utf::buffer_decoder<Char>::end() {
  return iterator(std::basic_string_view<Char>());
}

utf::buffer_decoder<char> utf::decoder(std::string_view str) {
  return buffer_decoder(str);
}

utf::buffer_decoder<char16_t> utf::decoder(std::u16string_view str) {
  return buffer_decoder(str);
}

// Appending a single code point, UTF-8
void utf::append(char*& str, char32_t chr) {
  if (chr < 0x80) *str++ = chr;
  else if (chr < 0x800) { *str++ = 0xC0 + (chr >> 6); *str++ = 0x80 + (chr & 0x3F); }
  else if (chr < 0x10000) { *str++ = 0xE0 + (chr >> 12); *str++ = 0x80 + ((chr >> 6) & 0x3F); *str++ = 0x80 + (chr & 0x3F); }
  else if (chr < 0x200000) { *str++ = 0xF0 + (chr >> 18); *str++ = 0x80 + ((chr >> 12) & 0x3F); *str++ = 0x80 + ((chr >> 6) & 0x3F); *str++ = 0x80 + (chr & 0x3F); }
  else *str++ = REPLACEMENT_CHAR;
}

void utf::append(std::string& str, char32_t chr) {
  if (chr < 0x80) str += chr;
  else if (chr < 0x800) { str += 0xC0 + (chr >> 6); str += 0x80 + (chr & 0x3F); }
  else if (chr < 0x10000) { str += 0xE0 + (chr >> 12); str += 0x80 + ((chr >> 6) & 0x3F); str += 0x80 + (chr & 0x3F); }
  else if (chr < 0x200000) { str += 0xF0 + (chr >> 18); str += 0x80 + ((chr >> 12) & 0x3F); str += 0x80 + ((chr >> 6) & 0x3F); str += 0x80 + (chr & 0x3F); }
  else str += REPLACEMENT_CHAR;
}

// Appending a single code point, UTF-16
void utf::append(char16_t*& str, char32_t chr) {
  if (chr <= 0xFFFF) *str++ = chr;
  else if (chr <= 0x10FFFF) { *str++ = 0xD800 + ((chr - 0x10000) >> 10); *str++ = 0xDC00 + ((chr - 0x10000) & 0x3FF); }
  else *str++ = REPLACEMENT_CHAR;
}

void utf::append(std::u16string& str, char32_t chr) {
  if (chr <= 0xFFFF) str += chr;
  else if (chr <= 0x10FFFF) { str += 0xD800 + ((chr - 0x10000) >> 10); str += 0xDC00 + ((chr - 0x10000) & 0x3FF); }
  else str += REPLACEMENT_CHAR;
}

// Encoding a whole string
void utf::encode(const char32_t* str, std::string& encoded) {
  encoded.clear();
  while (*str)
    append(encoded, *str++);
}

void utf::encode(std::u32string_view str, std::string& encoded) {
  encoded.clear();
  for (; !str.empty(); str.remove_prefix(1))
    append(encoded, str.front());
}

void utf::encode(const char32_t* str, std::u16string& encoded) {
  encoded.clear();
  while (*str)
    append(encoded, *str++);
}

void utf::encode(std::u32string_view str, std::u16string& encoded) {
  encoded.clear();
  for (; !str.empty(); str.remove_prefix(1))
    append(encoded, str.front());
}

} // namespace unilib
