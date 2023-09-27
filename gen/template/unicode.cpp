// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014-2023 Institute of Formal and Applied Linguistics, Faculty
// of Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// UniLib version: $UNILIB_VERSION
// Unicode version: $UNICODE_VERSION

#include "unicode.h"

namespace ufal {
namespace unilib {

const char32_t unicode::CHARS;

const int32_t unicode::DEFAULT_CAT;

const uint8_t unicode::category_index[unicode::CHARS >> 8] = $CATEGORY_INDICES;

const uint8_t unicode::category_block[][256] = $CATEGORY_BLOCKS;

const uint8_t unicode::othercase_index[unicode::CHARS >> 8] = $OTHERCASE_INDICES;

const char32_t unicode::othercase_block[][256] = $OTHERCASE_BLOCKS;

} // namespace unilib
} // namespace ufal
