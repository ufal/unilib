// This file is part of UniLib <http://github.com/ufal/unilib/>.
//
// Copyright 2014 Institute of Formal and Applied Linguistics, Faculty of
// Mathematics and Physics, Charles University in Prague, Czech Republic.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <cassert>

#include "test.h"

#include "unicode.h"
#include "utf.h"

using namespace unilib;

int main(void) {
  // UTF conversions
  auto u8_to_u32 = [](string str){ u32string res; utf::decode(str, res); return res; };
  auto u16_to_u32 = [](u16string str){ u32string res; utf::decode(str, res); return res; };
  auto u32_to_u16 = [](u32string str){ u16string res; utf::encode(str, res); return res; };
  auto u32_to_u8 = [](u32string str){ string res; utf::encode(str, res); return res; };

  // Iterators
  auto u8_str_iter = [](string str) { decltype(str) res; for (auto&& chr : utf::decoder(str)) utf::append(res, chr); return res; };
  auto u8_cstr_iter = [](string str) { decltype(str) res; for (auto&& chr : utf::decoder(str.c_str())) utf::append(res, chr); return res; };
  auto u16_str_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf::decoder(str)) utf::append(res, chr); return res; };
  auto u16_cstr_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf::decoder(str.c_str())) utf::append(res, chr); return res; };

  string line;
  vector<string> numbers;

  // Load UTF-32 data
  u32string u32;
  assert(getline(cin, line));
  split(line, ' ', numbers);
  for (const auto& number : numbers)
    u32.push_back(stoi(number));

  // Load UTF-16 data
  u16string u16;
  assert(getline(cin, line));
  split(line, ' ', numbers);
  for (const auto& number : numbers)
    u16.push_back(stoi(number));

  // Load UTF-8 data
  string u8;
  assert(getline(cin, line));
  split(line, ' ', numbers);
  for (const auto& number : numbers)
    u8.push_back(char(stoi(number)));

  test(u32_to_u8, u32, u8);
  test(u8_to_u32, u8, u32);
  test([=](string str) { return u32_to_u8(u8_to_u32(str)); }, u8, u8);
  test([=](u32string str) { return u8_to_u32(u32_to_u8(str)); }, u32, u32);
  test([=](u32string str) { return u8_to_u32(u8_str_iter(u32_to_u8(str))); }, u32, u32);
  test([=](u32string str) { return u8_to_u32(u8_cstr_iter(u32_to_u8(str))); }, u32, u32);

  test(u32_to_u16, u32, u16);
  test(u16_to_u32, u16, u32);
  test([=](u16string str) { return u32_to_u16(u16_to_u32(str)); }, u16, u16);
  test([=](u32string str) { return u16_to_u32(u32_to_u16(str)); }, u32, u32);
  test([=](u32string str) { return u16_to_u32(u16_str_iter(u32_to_u16(str))); }, u32, u32);
  test([=](u32string str) { return u16_to_u32(u16_cstr_iter(u32_to_u16(str))); }, u32, u32);

  return test_summary();
}
