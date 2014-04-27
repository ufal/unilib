// This file is part of UniLib.
//
// Copyright 2014 by Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// UniLib is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// UniLib is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with UniLib.  If not, see <http://www.gnu.org/licenses/>.

// UniLib version: $UNILIB_VERSION
// Unicode version: $UNICODE_VERSION

#pragma once

#include <cstddef>
#include <iterator>
#include <string>

namespace ufal {
namespace unilib {

class utf16 {
 public:
  static bool valid(const char16_t* str);
  static bool valid(const char16_t* str, size_t len);
  static inline bool valid(const std::u16string& str);

  static inline char32_t decode(const char16_t*& str);
  static inline char32_t decode(const char16_t*& str, size_t& len);
  static inline char32_t first(const char16_t* str);
  static inline char32_t first(const char16_t* str, size_t len);
  static inline char32_t first(const std::u16string& str);

  static void decode(const char16_t*& str, std::u32string& decoded);
  static void decode(const char16_t*& str, size_t& len, std::u32string& decoded);
  static inline void decode(const std::u16string& str, std::u32string& decoded);

  class string_decoder_helper {
   public:
    class iterator;
    inline iterator begin();
    inline iterator end();
   private:
    inline string_decoder_helper(const char16_t* str);
    const char16_t* str;
    friend class utf16;
  };
  static inline string_decoder_helper decoder(const char16_t* str);
  static inline string_decoder_helper decoder(const std::u16string& str);

  class buffer_decoder_helper {
   public:
    class iterator;
    inline iterator begin();
    inline iterator end();
   private:
    inline buffer_decoder_helper(const char16_t* str, size_t len);
    const char16_t* str;
    size_t len;
    friend class utf16;
  };
  static inline buffer_decoder_helper decoder(const char16_t* str, size_t len);

  static inline void append(char16_t*& str, char32_t chr);
  static inline void append(std::u16string& str, char32_t chr);

  static void encode(const std::u32string& str, std::u16string& encoded);

  template<class F> static void map(F f, const char16_t* str, std::u16string& result);
  template<class F> static void map(F f, const char16_t* str, size_t len, std::u16string& result);
  template<class F> static void map(F f, const std::u16string& str, std::u16string& result);

 private:
  static const char16_t REPLACEMENT_CHAR = '?';
};

bool utf16::valid(const std::u16string& str) {
  return valid(str.c_str());
}

char32_t utf16::decode(const char16_t*& str) {
  if (*str < 0xD800 || *str >= 0xE000) return *str++;
  if (*str >= 0xDC00) return ++str, REPLACEMENT_CHAR;
  char32_t res = 0x10000 + ((*str++ - 0xD800) << 10);
  if (*str < 0xDC00 || *str >= 0xE000) return REPLACEMENT_CHAR;
  return res + (*str++ - 0xDC00);
}

char32_t utf16::decode(const char16_t*& str, size_t& len) {
  if (!len) return 0; --len;
  if (*str < 0xD800 || *str >= 0xE000) return *str++;
  if (!len || *str >= 0xDC00) return ++str, REPLACEMENT_CHAR;
  char32_t res = 0x10000 + ((*str++ - 0xD800) << 10);
  if (*str < 0xDC00 || *str >= 0xE000) return REPLACEMENT_CHAR;
  return res + ((--len, *str++) - 0xDC00);
}

char32_t utf16::first(const char16_t* str) {
  return decode(str);
}

char32_t utf16::first(const char16_t* str, size_t len) {
  return decode(str, len);
}

char32_t utf16::first(const std::u16string& str) {
  return first(str.c_str());
}

void utf16::decode(const std::u16string& str, std::u32string& decoded) {
  decode(str.c_str(), decoded);
}

class utf16::string_decoder_helper::iterator : public std::iterator<std::input_iterator_tag, char32_t> {
 public:
  iterator(const char16_t* str) : codepoint(0), next(str) { operator++(); }
  iterator(const iterator& it) : codepoint(it.codepoint), next(it.next) {}
  iterator& operator++() { if (next) { codepoint = decode(next); if (!codepoint) next = nullptr; } return *this; }
  iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const iterator& other) const { return next == other.next; }
  bool operator!=(const iterator& other) const { return next != other.next; }
  const char32_t& operator*() { return codepoint; }
 private:
  char32_t codepoint;
  const char16_t* next;
};

utf16::string_decoder_helper::string_decoder_helper(const char16_t* str) : str(str) {}

utf16::string_decoder_helper::iterator utf16::string_decoder_helper::begin() {
  return iterator(str);
}

utf16::string_decoder_helper::iterator utf16::string_decoder_helper::end() {
  return iterator(nullptr);
}

utf16::string_decoder_helper utf16::decoder(const char16_t* str) {
  return string_decoder_helper(str);
}

utf16::string_decoder_helper utf16::decoder(const std::u16string& str) {
  return string_decoder_helper(str.c_str());
}

class utf16::buffer_decoder_helper::iterator : public std::iterator<std::input_iterator_tag, char32_t> {
 public:
  iterator(const char16_t* str, size_t len) : codepoint(0), next(str), len(len) { operator++(); }
  iterator(const iterator& it) : codepoint(it.codepoint), next(it.next), len(it.len) {}
  iterator& operator++() { if (next) { codepoint = decode(next, len); if (len <= 0) next = nullptr; } return *this; }
  iterator operator++(int) { iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const iterator& other) const { return next == other.next; }
  bool operator!=(const iterator& other) const { return next != other.next; }
  const char32_t& operator*() { return codepoint; }
 private:
  char32_t codepoint;
  const char16_t* next;
  size_t len;
};

utf16::buffer_decoder_helper::buffer_decoder_helper(const char16_t* str, size_t len) : str(str), len(len) {}

utf16::buffer_decoder_helper::iterator utf16::buffer_decoder_helper::begin() {
  return iterator(str, len);
}

utf16::buffer_decoder_helper::iterator utf16::buffer_decoder_helper::end() {
  return iterator(nullptr, 0);
}

utf16::buffer_decoder_helper utf16::decoder(const char16_t* str, size_t len) {
  return buffer_decoder_helper(str, len);
}

void utf16::append(char16_t*& str, char32_t chr) {
  if (chr <= 0xFFFF) *str++ = chr;
  else if (chr <= 0x10FFFF) { *str++ = 0xD800 + ((chr - 0x10000) >> 10); *str++ = 0xDC00 + ((chr - 0x10000) & 0x3FF); }
  else *str++ = REPLACEMENT_CHAR;
}

void utf16::append(std::u16string& str, char32_t chr) {
  if (chr <= 0xFFFF) str += chr;
  else if (chr <= 0x10FFFF) { str += 0xD800 + ((chr - 0x10000) >> 10); str += 0xDC00 + ((chr - 0x10000) & 0x3FF); }
  else str += REPLACEMENT_CHAR;
}

template<class F> void utf16::map(F f, const char16_t* str, std::u16string& result) {
  result.clear();

  for (char32_t chr; (chr = decode(str)); )
    append(result, f(chr));
}

template<class F> void utf16::map(F f, const char16_t* str, size_t len, std::u16string& result) {
  result.clear();

  while (len)
    append(result, f(decode(str, len)));
}

template<class F> void utf16::map(F f, const std::u16string& str, std::u16string& result) {
  map(f, str.c_str(), result);
}

} // namespace unilib
} // namespace ufal