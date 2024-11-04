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

#include <cstdint>

namespace unilib {

// The `unistrip` class performs combining marks stripping. The same
// functionality can be achieved by combining `unicode` and `uninorms`
// classes, but `unistrip` class performs the task much faster and
// needs less embedded data.
class unistrip {
 public:
  // Returns `true` if the category of the given Unicode code point is `M`
  // (i.e., nonspacing mark `Mn` or spacing mark `Mc` or enclosing `Me`).
  static inline bool is_combining_mark(char32_t chr);

  // Returns Unicode code point which has all combining marks removed. If the
  // given code point is a combining mark itself, or there are no combining
  // marks presents, `strip_combining_marks` returns the original code point.
  //
  // This method is equivalent to the following process:
  // - If the given code point is outside the valid Unicode range, return it.
  // - If the given code points has general category `M`, return it.
  // - Othervise, convert it to NFD.
  // - If there are no code points with general category `M`, return
  //   the original code point.
  // - Otherwise, remove all code points with general cagetory `M`.
  // - Convert the result to NFC.
  // - Return the resulting code point.
  static inline char32_t strip_combining_marks(char32_t chr);

 private:
  inline static const char32_t CHARS = 0x110000;

  static const uint8_t combining_mark_index[CHARS >> 8];
  static const uint8_t combining_mark_block[][32];

  static const uint8_t stripped_index[CHARS >> 8];
  static const uint16_t stripped_block[][256];
  static const char32_t stripped_data[];
};

bool unistrip::is_combining_mark(char32_t chr) {
  return chr < CHARS && combining_mark_block[combining_mark_index[chr >> 8]][(chr >> 3) & 0x1F] & (uint8_t(1) << (chr & 0x07));
}

char32_t unistrip::strip_combining_marks(char32_t chr) {
  if (chr >= CHARS) return chr;
  uint16_t index = stripped_block[stripped_index[chr >> 8]][chr & 0xFF];
  return index ? stripped_data[index] : chr;
}

} // namespace unilib
