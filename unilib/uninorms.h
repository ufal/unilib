// This file is part of UniLib <http://github.com/ufal/unilib/>.
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

// UniLib version: 1.1
// Unicode version: 7.0.0

#pragma once

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
