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
