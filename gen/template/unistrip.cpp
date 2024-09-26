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

#include "unistrip.h"

namespace unilib {

const uint8_t unistrip::combining_mark_index[unistrip::CHARS >> 8] = $COMBINING_MARK_INDICES;

const uint8_t unistrip::combining_mark_block[][32] = $COMBINING_MARK_BLOCKS;

const uint8_t unistrip::stripped_index[unistrip::CHARS >> 8] = $STRIPPED_INDICES;

const uint16_t unistrip::stripped_block[][256] = $STRIPPED_BLOCKS;

const char32_t unistrip::stripped_data[] = $STRIPPED_DATA;

} // namespace unilib
