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
#include <string>

namespace unilib {

// The `uninorms` class performs normalization to [Unicode Normalization
// Forms](http://unicode.org/reports/tr15/). All normalization forms are
// supported and the normalization is performed in-place on a `u32string`
// representing a string of Unicode code points.
class uninorms {
 public:
  // Convert given string to [Normalization Form C](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfc(std::u32string& str);

  // Convert given string to [Normalization Form D](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfd(std::u32string& str);

  // Convert given string to [Normalization Form KC](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfkc(std::u32string& str);

  // Convert given string to [Normalization Form KD](http://unicode.org/reports/tr15/#Norm_Forms).
  static void nfkd(std::u32string& str);

 private:
  static void compose(std::u32string& str);
  static void decompose(std::u32string& str, bool kanonical);

  inline static const char32_t CHARS = 0x110000;

  struct Hangul {
    // Hangul decomposition and composition
    inline static const char32_t SBase = 0xAC00, LBase = 0x1100, VBase = 0x1161, TBase = 0x11A7;
    inline static const char32_t LCount = 19, VCount = 21, TCount = 28, NCount = VCount * TCount, SCount = LCount * NCount;
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
