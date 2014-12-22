// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014 Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <algorithm>

#include "test.h"

#include "unicode.h"
#include "uninorms.h"
#include "unistrip.h"

using namespace ufal::unilib;

int main(void) {
  // Combining mark tests
  for (char32_t chr = 0; chr < 0x110000; chr++) {
    // Check that is_combining_mark corresponds with General Category M.
    test(unistrip::is_combining_mark, chr, (unicode::category(chr) & unicode::M) ? true : false);

    if (!unistrip::is_combining_mark(chr)) {
      // Check that NFD + remove General Category M + NFC equals to strip_combining_marks.
      char32_t stripped = chr;
      u32string str(1, chr);
      uninorms::nfd(str);
      if (count_if(str.begin(), str.end(), unistrip::is_combining_mark)) {
        str.erase(remove_if(str.begin(), str.end(), unistrip::is_combining_mark), str.end());
        uninorms::nfc(str);

        test([](const u32string& str){ return str.length(); }, str, 1);
        if (str.length() == 1) stripped = str[0];
      }
      test(unistrip::strip_combining_marks, chr, stripped);
    }
  }

  test_summary();

  return 0;
}
