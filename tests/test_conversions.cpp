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
#include "utf8.h"
#include "utf16.h"

using namespace ufal::unilib;

#ifndef _WIN32
// Unfortunately, std::codecvt<char32_t, char> and std::codecvt<char16_t, char>
// do not work on Windows (known bug present in VS 2015 and VS 2017). We therefore
// use our own routines, so the test does not do much.

#include <cassert>
#include <codecvt>
#include <locale>

string std_u32_to_u8(u32string in) {
  static auto& convertor = use_facet<codecvt<char32_t, char, mbstate_t>>(locale::classic());

  mbstate_t mb{};
  const char32_t* from_next;
  char* to_next;

  string out(in.size() * convertor.max_length(), '\0');
  assert(convertor.out(mb, &in[0], &in[in.size()], from_next, &out[0], &out[out.size()], to_next) == codecvt_base::ok);
  out.resize(to_next - &out[0]);
  return out;
}

u32string std_u8_to_u32(string in) {
  static auto& convertor = use_facet<codecvt<char32_t, char, mbstate_t>>(locale::classic());

  mbstate_t mb{};
  const char* from_next;
  char32_t* to_next;

  u32string out(in.size(), U'\0');
  assert(convertor.in(mb, &in[0], &in[in.size()], from_next, &out[0], &out[out.size()], to_next) == codecvt_base::ok);
  out.resize(to_next - &out[0]);
  return out;
}

u16string std_u32_to_u16(u32string in) {
  static auto& convertor = use_facet<codecvt<char16_t, char, mbstate_t>>(locale::classic());

  string u8_in = std_u32_to_u8(in);

  mbstate_t mb{};
  const char* from_next;
  char16_t* to_next;

  u16string out(u8_in.size(), u'\0');
  assert(convertor.in(mb, &u8_in[0], &u8_in[u8_in.size()], from_next, &out[0], &out[out.size()], to_next) == codecvt_base::ok);
  out.resize(to_next - &out[0]);
  return out;
}

u32string std_u16_to_u32(u16string in) {
  static auto& convertor = use_facet<codecvt<char16_t, char, mbstate_t>>(locale::classic());

  mbstate_t mb{};
  const char16_t* from_next;
  char* to_next;

  string out(in.size() * convertor.max_length(), '\0');
  assert(convertor.out(mb, &in[0], &in[in.size()], from_next, &out[0], &out[out.size()], to_next) == codecvt_base::ok);
  out.resize(to_next - &out[0]);

  return std_u8_to_u32(out);
}

#else

string std_u32_to_u8(u32string in) {
  string out;
  utf8::encode(in, out);
  return out;
}

u32string std_u8_to_u32(string in) {
  u32string out;
  utf8::decode(in, out);
  return out;
}

u16string std_u32_to_u16(u32string in) {
  u16string out;
  utf16::encode(in, out);
  return out;
}

u32string std_u16_to_u32(u16string in) {
  u32string out;
  utf16::decode(in, out);
  return out;
}

#endif

int main(void) {
  // UTF conversions
  auto u8_to_u32 = [](string str){ u32string res; utf8::decode(str, res); return res; };
  auto u16_to_u32 = [](u16string str){ u32string res; utf16::decode(str, res); return res; };
  auto u32_to_u16 = [](u32string str){ u16string res; utf16::encode(str, res); return res; };
  auto u32_to_u8 = [](u32string str){ string res; utf8::encode(str, res); return res; };

  // Iterators
  auto u8_str_iter = [](string str) { decltype(str) res; for (auto&& chr : utf8::decoder(str)) utf8::append(res, chr); return res; };
  auto u8_cstr_iter = [](string str) { decltype(str) res; for (auto&& chr : utf8::decoder(str.c_str())) utf8::append(res, chr); return res; };
  auto u8_buffer_iter = [](string str) { decltype(str) res; for (auto&& chr : utf8::decoder(str.c_str(), str.size())) utf8::append(res, chr); return res; };
  auto u16_str_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf16::decoder(str)) utf16::append(res, chr); return res; };
  auto u16_cstr_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf16::decoder(str.c_str())) utf16::append(res, chr); return res; };
  auto u16_buffer_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf16::decoder(str.c_str(), str.size())) utf16::append(res, chr); return res; };

  // Conversion tests for all non-Cn and non-Cs code points
  u32string codepoints;
  for (char32_t chr = 1; chr < 0x110000; chr++)
    if (unicode::category(chr) & ~(unicode::Cn | unicode::Cs))
      codepoints.push_back(chr);

  test(u32_to_u8, codepoints, std_u32_to_u8(codepoints));
  test(u8_to_u32, std_u32_to_u8(codepoints), codepoints);
  test([=](u32string str) { return u8_to_u32(u32_to_u8(str)); }, codepoints, codepoints);
  test([=](u32string str) { return u8_to_u32(u8_str_iter(u32_to_u8(str))); }, codepoints, codepoints);
  test([=](u32string str) { return u8_to_u32(u8_cstr_iter(u32_to_u8(str))); }, codepoints, codepoints);
  test([=](u32string str) { return u8_to_u32(u8_buffer_iter(u32_to_u8(str))); }, codepoints, codepoints);

  test(u32_to_u16, codepoints, std_u32_to_u16(codepoints));
  test(u16_to_u32, std_u32_to_u16(codepoints), codepoints);
  test([=](u32string str) { return u16_to_u32(u32_to_u16(str)); }, codepoints, codepoints);
  test([=](u32string str) { return u16_to_u32(u16_str_iter(u32_to_u16(str))); }, codepoints, codepoints);
  test([=](u32string str) { return u16_to_u32(u16_cstr_iter(u32_to_u16(str))); }, codepoints, codepoints);
  test([=](u32string str) { return u16_to_u32(u16_buffer_iter(u32_to_u16(str))); }, codepoints, codepoints);

  return test_summary();
}
