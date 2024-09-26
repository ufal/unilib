// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014-2024 Institute of Formal and Applied Linguistics, Faculty
// of Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// UniLib version: $UNILIB_VERSION
// Unicode version: $UNICODE_VERSION

#include "unicode.h"

namespace unilib {

std::u32string unicode::lowercase_full(char32_t chr) {
  switch (chr) {
    $LOWERCASES_FULL
  }
  return std::u32string(1, unicode::lowercase(chr));
}

std::u32string unicode::uppercase_full(char32_t chr) {
  switch (chr) {
    $UPPERCASES_FULL
  }
  return std::u32string(1, unicode::uppercase(chr));
}

std::u32string unicode::titlecase_full(char32_t chr) {
  switch (chr) {
    $TITLECASES_FULL
  }
  return std::u32string(1, unicode::titlecase(chr));
}

const uint8_t unicode::category_index[unicode::CHARS >> 8] = $CATEGORY_INDICES;

const uint8_t unicode::category_block[][256] = $CATEGORY_BLOCKS;

const uint8_t unicode::othercase_index[unicode::CHARS >> 8] = $OTHERCASE_INDICES;

const char32_t unicode::othercase_block[][256] = $OTHERCASE_BLOCKS;

} // namespace unilib
