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

#include "uninorms.h"

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
  size_t old, com;
  for (old = 0, com = 0; old < str.size(); old++, com++) {
    str[com] = str[old];
    if (str[old] >= Hangul::LBase && str[old] < Hangul::LBase + Hangul::LCount) {
      // Check Hangul composition L + V [+ T].
      if (old + 1 < str.size() && str[old + 1] >= Hangul::VBase && str[old + 1] < Hangul::VBase + Hangul::VCount) {
        str[com] = Hangul::SBase + ((str[old] - Hangul::LBase) * Hangul::VCount + str[old + 1] - Hangul::VBase) * Hangul::TCount;
        old++;
        if (old + 1 < str.size() && str[old + 1] > Hangul::TBase && str[old + 1] < Hangul::TBase + Hangul::TCount)
          str[com] += str[++old] - Hangul::TBase;
      }
    } else if (str[old] >= Hangul::SBase && str[old] < Hangul::SBase + Hangul::SCount) {
      // Check Hangul composition LV + T
      if ((str[old] - Hangul::SBase) % Hangul::TCount && old + 1 < str.size() && str[old + 1] > Hangul::TBase && str[old + 1] < Hangul::TBase + Hangul::TCount)
        str[com] += str[++old] - Hangul::TBase;
    } else if (str[old] < CHARS) {
      // Check composition_data
      auto composition = &composition_block[composition_index[str[old] >> 8]][str[old] & 0xFF];
      auto starter = com;
      for (int last_ccc = -1; old + 1 < str.size(); old++) {
        int ccc = str[old + 1] < CHARS ? ccc_block[ccc_index[str[old + 1] >> 8]][str[old + 1] & 0xFF] : 0;
        if (composition[1] - composition[0] && last_ccc < ccc) {
          // Try finding a composition.
          auto l = composition[0], r = composition[1];
          while (l + 2 < r) {
            auto m = l + (((r - l) >> 1) & ~1);
            if (composition_data[m] <= str[old + 1]) l = m;
            if (composition_data[m] >= str[old + 1]) r = m;
          }
          if (composition_data[l] == str[old + 1]) {
            // Found a composition.
            str[starter] = composition_data[l + 1];
            composition = &composition_block[composition_index[composition_data[l + 1] >> 8]][composition_data[l + 1] & 0xFF];
            continue;
          }
        }

        if (!ccc) break;
        last_ccc = ccc;
        str[++com] = str[old + 1];
      }
    }
  }

  if (com < old) str.resize(com);
}

void uninorms::decompose(std::u32string& str, bool kompatibility) {
  // Count how much additional space do we need.
  bool any_decomposition = false;
  size_t additional = 0;
  for (auto&& chr : str) {
    int decomposition_len = 0;

    if (chr >= Hangul::SBase && chr < Hangul::SBase + Hangul::SCount) {
      // Hangul decomposition.
      decomposition_len = 2 + ((chr - Hangul::SBase) % Hangul::TCount ? 1 : 0);
    } else if (chr < CHARS) {
      // Check decomposition_data.
      auto decomposition = &decomposition_block[decomposition_index[chr >> 8]][chr & 0xFF];
      decomposition_len = (decomposition[1] >> 2) - (decomposition[0] >> 2);
      if (decomposition_len && !kompatibility && (decomposition[0] & 1)) decomposition_len = 0;
      if (decomposition_len && kompatibility && (decomposition[0] & 2))
        // Further kompatibility decomposition.
        for (auto i = decomposition[0] >> 2; i < decomposition[1] >> 2; i++) {
          auto further_decomposition = &decomposition_block[decomposition_index[decomposition_data[i] >> 8]][decomposition_data[i] & 0xFF];
          if (further_decomposition[0] & 1) decomposition_len += (further_decomposition[1] >> 2) - (further_decomposition[0] >> 2) - 1;
        }
    }
    // Do we decompose current character?
    if (!decomposition_len) continue;
    any_decomposition = true;
    additional += decomposition_len - 1;
  }

  // If needed, allocate enough space and perform the decomposition.
  if (any_decomposition) {
    str.resize(str.size() + additional);
    for (size_t dec = str.size(), old = dec - additional; old--; )
      if (str[old] >= Hangul::SBase && str[old] < Hangul::SBase + Hangul::SCount) {
        // Hangul decomposition.
        char32_t s_index = str[old] - Hangul::SBase;
        if (s_index % Hangul::TCount) str[--dec] = Hangul::TBase + s_index % Hangul::TCount;
        str[--dec] = Hangul::VBase + (s_index % Hangul::NCount) / Hangul::TCount;
        str[--dec] = Hangul::LBase + s_index / Hangul::NCount;
      } else if (str[old] < CHARS) {
        // Check decomposition_data.
        auto decomposition = &decomposition_block[decomposition_index[str[old] >> 8]][str[old] & 0xFF];
        int decomposition_len = (decomposition[1] >> 2) - (decomposition[0] >> 2);
        if (decomposition_len && !kompatibility && (decomposition[0] & 1)) decomposition_len = 0;
        if (decomposition_len && kompatibility && (decomposition[0] & 2)) {
          // Further kompatibility decomposition.
          while (decomposition_len--) {
            auto chr = decomposition_data[(decomposition[0] >> 2) + decomposition_len];
            auto further_decomposition = &decomposition_block[decomposition_index[chr >> 8]][chr & 0xFF];
            if (further_decomposition[0] & 1) {
              for (int further_decomposition_len = (further_decomposition[1] >> 2) - (further_decomposition[0] >> 2); further_decomposition_len--; )
                str[--dec] = decomposition_data[(further_decomposition[0] >> 2) + further_decomposition_len];
            } else {
              str[--dec] = chr;
            }
          }
        } else if (decomposition_len) {
          // Non-recursive decomposition.
          while (decomposition_len--)
            str[--dec] = decomposition_data[(decomposition[0] >> 2) + decomposition_len];
        } else {
          // No decomposition.
          str[--dec] = str[old];
        }
      } else {
        // Non-Unicode character.
        str[--dec] = str[old];
      }
  }

  // Sort combining marks.
  for (size_t i = 1; i < str.size(); i++) {
    unsigned ccc = str[i] < CHARS ? ccc_block[ccc_index[str[i] >> 8]][str[i] & 0xFF] : 0;
    if (!ccc) continue;

    auto chr = str[i];
    size_t j;
    for (j = i; j && (str[j-1] < CHARS ? ccc_block[ccc_index[str[j-1] >> 8]][str[j-1] & 0xFF] : 0) > ccc; j--) str[j] = str[j-1];
    str[j] = chr;
  }
}

// Data fields
const uint8_t uninorms::ccc_index[uninorms::CHARS >> 8] = $CCC_INDICES;
const uint8_t uninorms::ccc_block[][256] = $CCC_BLOCKS;

const uint8_t uninorms::composition_index[uninorms::CHARS >> 8] = $COMPOSITION_INDICES;
const uint16_t uninorms::composition_block[][257] = $COMPOSITION_BLOCKS;
const char32_t uninorms::composition_data[] = $COMPOSITION_DATA;

const uint8_t uninorms::decomposition_index[uninorms::CHARS >> 8] = $DECOMPOSITION_INDICES;
const uint16_t uninorms::decomposition_block[][257] = $DECOMPOSITION_BLOCKS;
const char32_t uninorms::decomposition_data[] = $DECOMPOSITION_DATA;

} // namespace unilib
