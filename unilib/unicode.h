// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014-2024 Institute of Formal and Applied Linguistics, Faculty
// of Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// UniLib version: 4.2.1-dev
// Unicode version: 17.0.0

#pragma once

#include <cstdint>
#include <string>

namespace unilib {

// The `unicode` class contains basic information from
// [Unicode Character Database](http://www.unicode.org/reports/tr44/), notably
// - general categories
// - simple and full uppercase/lowercase/titlecase mappings
class unicode {
  enum : uint8_t {
    _Lu = 1, _Ll = 2, _Lt = 3, _Lm = 4, _Lo = 5,
    _Mn = 6, _Mc = 7, _Me = 8,
    _Nd = 9, _Nl = 10, _No = 11,
    _Pc = 12, _Pd = 13, _Ps = 14, _Pe = 15, _Pi = 16, _Pf = 17, _Po = 18,
    _Sm = 19, _Sc = 20, _Sk = 21, _So = 22,
    _Zs = 23, _Zl = 24, _Zp = 25,
    _Cc = 26, _Cf = 27, _Cs = 28, _Co = 29, _Cn = 30
  };

 public:
  // The `category_t` type represents general category from
  // [Unicode Character Database](http://www.unicode.org/reports/tr44/).
  // Its values are:
  // - abbreviated property value aliases for general categories,
  // - abbreviated aliases for groupings of related general categories.
  //
  // Note that the `category_t` is a bitmask indicating general categories,
  // which makes it easy to define arbitrary subsets of general categories
  // similarly to how groupings of general categories are defined.
  typedef uint32_t category_t;
  enum : category_t {
    Lu = 1 << _Lu, Ll = 1 << _Ll, Lt = 1 << _Lt, Lut = Lu | Lt, LC = Lu | Ll | Lt,
      Lm = 1 << _Lm, Lo = 1 << _Lo, L = Lu | Ll | Lt | Lm | Lo,
    Mn = 1 << _Mn, Mc = 1 << _Mc, Me = 1 << _Me, M = Mn | Mc | Me,
    Nd = 1 << _Nd, Nl = 1 << _Nl, No = 1 << _No, N = Nd | Nl | No,
    Pc = 1 << _Pc, Pd = 1 << _Pd, Ps = 1 << _Ps, Pe = 1 << _Pe, Pi = 1 << _Pi,
      Pf = 1 << _Pf, Po = 1 << _Po, P = Pc | Pd | Ps | Pe | Pi | Pf | Po,
    Sm = 1 << _Sm, Sc = 1 << _Sc, Sk = 1 << _Sk, So = 1 << _So, S = Sm | Sc | Sk | So,
    Zs = 1 << _Zs, Zl = 1 << _Zl, Zp = 1 << _Zp, Z = Zs | Zl | Zp,
    Cc = 1 << _Cc, Cf = 1 << _Cf, Cs = 1 << _Cs, Co = 1 << _Co, Cn = 1 << _Cn, C = Cc | Cf | Cs | Co | Cn
  };

  // Returns general category of the given Unicode code point as a value of
  // `category_t` enumeration. The `Cn` category is returned for code points
  // outside of valid range.
  static inline category_t category(char32_t chr);

  // Returns the simple lowercase/uppercase/titlecase mapping of the given code
  // point from [Unicode Character Database](http://www.unicode.org/reports/tr44/).
  // If no such mapping is defined or the given code point is outside of valid
  // range, the original code point is returned.
  static inline char32_t lowercase(char32_t chr);
  static inline char32_t uppercase(char32_t chr);
  static inline char32_t titlecase(char32_t chr);

  // Returns the full lowercase/uppercase/titlecase mapping of the given code
  // point from [Unicode Character Database](http://www.unicode.org/reports/tr44/).
  // If no full mapping is defined, return the corresponding simple mapping.
  static std::u32string lowercase_full(char32_t chr);
  static std::u32string uppercase_full(char32_t chr);
  static std::u32string titlecase_full(char32_t chr);

 private:
  inline static const char32_t CHARS = 0x110000;
  inline static const int32_t DEFAULT_CAT = Cn;

  static const uint8_t category_index[CHARS >> 8];
  static const uint8_t category_block[][256];
  static const uint8_t othercase_index[CHARS >> 8];
  static const char32_t othercase_block[][256];

  enum othercase_type { LOWER_ONLY = 1, UPPERTITLE_ONLY = 2, UPPER_ONLY = 3, LOWER_THEN_UPPER = 4, UPPER_THEN_TITLE = 5, TITLE_THEN_LOWER = 6 };
};

unicode::category_t unicode::category(char32_t chr) {
  return chr < CHARS ? 1 << category_block[category_index[chr >> 8]][chr & 0xFF] : DEFAULT_CAT;
}

char32_t unicode::lowercase(char32_t chr) {
  if (chr < CHARS) {
    char32_t othercase = othercase_block[othercase_index[chr >> 8]][chr & 0xFF];
    if ((othercase & 0xFF) == othercase_type::LOWER_ONLY) return othercase >> 8;
    if ((othercase & 0xFF) == othercase_type::LOWER_THEN_UPPER) return othercase >> 8;
    if ((othercase & 0xFF) == othercase_type::TITLE_THEN_LOWER) return othercase_block[othercase_index[(othercase >> 8) >> 8]][(othercase >> 8) & 0xFF] >> 8;
  }
  return chr;
}

char32_t unicode::uppercase(char32_t chr) {
  if (chr < CHARS) {
    char32_t othercase = othercase_block[othercase_index[chr >> 8]][chr & 0xFF];
    if ((othercase & 0xFF) == othercase_type::UPPERTITLE_ONLY) return othercase >> 8;
    if ((othercase & 0xFF) == othercase_type::UPPER_ONLY) return othercase >> 8;
    if ((othercase & 0xFF) == othercase_type::UPPER_THEN_TITLE) return othercase >> 8;
    if ((othercase & 0xFF) == othercase_type::LOWER_THEN_UPPER) return othercase_block[othercase_index[(othercase >> 8) >> 8]][(othercase >> 8) & 0xFF] >> 8;
  }
  return chr;
}

char32_t unicode::titlecase(char32_t chr) {
  if (chr < CHARS) {
    char32_t othercase = othercase_block[othercase_index[chr >> 8]][chr & 0xFF];
    if ((othercase & 0xFF) == othercase_type::UPPERTITLE_ONLY) return othercase >> 8;
    if ((othercase & 0xFF) == othercase_type::TITLE_THEN_LOWER) return othercase >> 8;
    if ((othercase & 0xFF) == othercase_type::UPPER_THEN_TITLE) return othercase_block[othercase_index[(othercase >> 8) >> 8]][(othercase >> 8) & 0xFF] >> 8;
  }
  return chr;
}

} // namespace unilib
