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

#include "uninorms.h"

namespace ufal {
namespace unilib {

void uninorms::nfc(std::u32string& str) {
  decompose(str, false);
  compose(str);
}

void uninorms::nfd(std::u32string& str) {
  decompose(str, false);
}

void uninorms::nfkc(std::u32string& str) {
  decompose(str, true);
  compose(str);
}

void uninorms::nfkd(std::u32string& str) {
  decompose(str, true);
}

void uninorms::compose(std::u32string& str) {
}

void uninorms::decompose(std::u32string& str, bool kanonical) {
}

const char32_t uninorms::CHARS;

const uint8_t uninorms::ccc_index[uninorms::CHARS >> 8] = $CCC_INDICES;

const uint8_t uninorms::ccc_block[][256] = $CCC_BLOCKS;

const uint8_t uninorms::composition_index[uninorms::CHARS >> 8] = $COMPOSITION_INDICES;

const uint16_t uninorms::composition_block[][257] = $COMPOSITION_BLOCKS;

const char32_t uninorms::composition_data[] = $COMPOSITION_DATA;

const uint8_t uninorms::decomposition_index[uninorms::CHARS >> 8] = $DECOMPOSITION_INDICES;

const uint16_t uninorms::decomposition_block[][257] = $DECOMPOSITION_BLOCKS;

const char32_t uninorms::decomposition_data[] = $DECOMPOSITION_DATA;

} // namespace unilib
} // namespace ufal
