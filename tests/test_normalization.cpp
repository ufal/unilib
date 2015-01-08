// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014 Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vector>

#include "test.h"

#include "uninorms.h"
using namespace ufal::unilib;

void split(const string& text, char sep, vector<string>& tokens) {
  tokens.clear();
  if (text.empty()) return;

  string::size_type index = 0;
  for (string::size_type next; (next = text.find(sep, index)) != string::npos; index = next + 1)
    tokens.emplace_back(text, index, next - index);
  tokens.emplace_back(text, index);
}

int main(void) {
  // Unicode normalization
  auto nfc = [](u32string str) { uninorms::nfc(str); return str; };
  auto nfd = [](u32string str) { uninorms::nfd(str); return str; };
  auto nfkc = [](u32string str) { uninorms::nfkc(str); return str; };
  auto nfkd = [](u32string str) { uninorms::nfkd(str); return str; };

  // Simple NFD + NFKD tests
  for (auto&& data : {
    make_tuple(U("příliš žluťoučký kůň úpěl ďábelské ódy"),
               U("pr\u030ci\u0301lis\u030c z\u030clut\u030couc\u030cky\u0301 ku\u030an\u030c u\u0301pe\u030cl d\u030ca\u0301belske\u0301 o\u0301dy")),
    make_tuple(U("PŘÍLIŠ ŽLUŤOUČKÝ KŮŇ ÚPĚL ĎÁBELSKÉ ÓDY"),
               U("PR\u030cI\u0301LIS\u030c Z\u030cLUT\u030cOUC\u030cKY\u0301 KU\u030aN\u030c U\u0301PE\u030cL D\u030cA\u0301BELSKE\u0301 O\u0301DY")),
    make_tuple(U("\u0345\u035D\u035C\u0315\u0300\u05AE\U0001D16D\u302E\u059A\u0316\u302A\u031B\u1DCE\u0321\u0F74\u0F72\u0F71\u0EC8"),
               U("\u0EC8\u0F71\u0F72\u0F74\u0321\u1DCE\u031B\u302A\u0316\u059A\u302E\U0001D16D\u05AE\u0300\u0315\u035C\u035D\u0345")),
    make_tuple(U("\u0EB8\u0E48\u0E38\u0C56\u0C55\u0711\u0670\u0652\u0651\u061A\u0619\u0618\u064D\u064C\u064B\uFB1E\u05C2\u05C1"),
               U("\u05C1\u05C2\uFB1E\u064B\u064C\u064D\u0618\u0619\u061A\u0651\u0652\u0670\u0711\u0C55\u0C56\u0E38\u0E48\u0EB8")),
    make_tuple(U("\u05BF\u05BD\u05BC\u05BB\u05B9\u05B8\u05B7\u05B6\u05B5\u05B4\u05B3\u05B2\u05B1\u05B0\u094D\u3099\u093C\u0334"),
               U("\u0334\u093C\u3099\u094D\u05B0\u05B1\u05B2\u05B3\u05B4\u05B5\u05B6\u05B7\u05B8\u05B9\u05BB\u05BC\u05BD\u05BF"))}) {
    test(nfd, get<0>(data).u32, get<1>(data).u32);
    test(nfd, get<1>(data).u32, get<1>(data).u32);
    test(nfkd, get<0>(data).u32, get<1>(data).u32);
    test(nfkd, get<1>(data).u32, get<1>(data).u32);
  }

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
