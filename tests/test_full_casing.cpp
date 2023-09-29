// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014 Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "test.h"

#include "unicode.h"
using namespace ufal::unilib;

int main(void) {
  vector full_casing_methods = {unicode::lowercase_full, unicode::titlecase_full, unicode::uppercase_full};

  // Load the SpecialCasing file
  string line;
  vector<string> columns, codepoints;
  while (getline(cin, line)) {
    if (line.empty() || line.compare(0, 1, "#") == 0) continue;

    split(line, ';', columns);
    if (columns.size() == 6) continue; // Ignore language-specific mappings.
    if (columns.size() != 5) return cerr << "Cannot parse SpecialCasing line " << line << endl, 1;

    char32_t code = stoi(columns[0], nullptr, 16);

    for (size_t i = 0; i < full_casing_methods.size(); i++) {
      u32string correct;
      split(columns[1 + i], ' ', codepoints);
      for (const auto& codepoint : codepoints)
        if (!codepoint.empty())
          correct.push_back(stoi(codepoint, nullptr, 16));

      test(full_casing_methods[i], code, correct);
    }
  }

  return test_summary();
}
