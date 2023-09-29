// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014 Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test.h"

#include "uninorms.h"
using namespace unilib;

int main(void) {
  // Unicode normalization
  auto nfc = [](u32string str) { uninorms::nfc(str); return str; };
  auto nfd = [](u32string str) { uninorms::nfd(str); return str; };
  auto nfkc = [](u32string str) { uninorms::nfkc(str); return str; };
  auto nfkd = [](u32string str) { uninorms::nfkd(str); return str; };

  // Load UCD NormalizationTest from standard input and perform required
  // conformance tests.
  string line;
  vector<string> words;
  vector<string> characters;
  vector<u32string> forms(5);
  vector<bool> tested_chars(0x110000);
  while (getline(cin, line)) {
    split(line, ';', words);
    if (words.size() != forms.size()) return cerr << "Cannot parse NormalizationTest line " << line << endl, 1;
    for (unsigned i = 0; i < words.size(); i++) {
      split(words[i], ' ', characters);
      forms[i].clear();
      for (auto&& character : characters)
        forms[i] += stoi(character, nullptr, 16);
    }

    test(nfc, forms[0], forms[1]);
    test(nfc, forms[1], forms[1]);
    test(nfc, forms[2], forms[1]);
    test(nfc, forms[3], forms[3]);
    test(nfc, forms[4], forms[3]);

    test(nfd, forms[0], forms[2]);
    test(nfd, forms[1], forms[2]);
    test(nfd, forms[2], forms[2]);
    test(nfd, forms[3], forms[4]);
    test(nfd, forms[4], forms[4]);

    test(nfkc, forms[0], forms[3]);
    test(nfkc, forms[1], forms[3]);
    test(nfkc, forms[2], forms[3]);
    test(nfkc, forms[3], forms[3]);
    test(nfkc, forms[4], forms[3]);

    test(nfkd, forms[0], forms[4]);
    test(nfkd, forms[1], forms[4]);
    test(nfkd, forms[2], forms[4]);
    test(nfkd, forms[3], forms[4]);
    test(nfkd, forms[4], forms[4]);

    if (forms[0].size() == 1) tested_chars[forms[0][0]] = true;
  }

  // One-letter strings not mentioned in NormalizationTest are fixed points.
  u32string letter(U" ");
  for (char32_t chr = 0; chr < tested_chars.size(); chr++)
    if (!tested_chars[chr]) {
      letter[0] = chr;
      test(nfc, letter, letter);
      test(nfd, letter, letter);
      test(nfkc, letter, letter);
      test(nfkd, letter, letter);
    }

  return test_summary();
}
