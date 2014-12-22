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

int main(void) {
  // UTF conversions
  auto u8_to_u32 = [](string str){ u32string res; utf8::decode(str, res); return res; };
  auto u16_to_u32 = [](u16string str){ u32string res; utf16::decode(str, res); return res; };
  auto u32_to_u16 = [](u32string str){ u16string res; utf16::encode(str, res); return res; };
  auto u32_to_u8 = [](u32string str){ string res; utf8::encode(str, res); return res; };

  // UTF conversions tests
  for (auto&& data : {
    U("Příliš žluťoučký kůň úpěl ďábelské ódy"),
    U("あめ つち ほし そら / やま かは みね たに / くも きり むろ こけ / ひと いぬ うへ すゑ / ゆわ さる おふ せよ / えのえを なれ ゐて"),
    U("В чащах юга жил бы цитрус? Да, но фальшивый экземпляръ!"),
    U("صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ"),
    U("דג סקרן שט בים מאוכזב ולפתע מצא חברה"),
    U("𐐀𐐁𐐂𐐃𐐄𐐅𐐆𐐇𐐈𐐉𐐊𐐋𐐌𐐍𐐎𐐏𐐐𐐑𐐒𐐓𐐔𐐕𐐖𐐗𐐘𐐙𐐚𐐛𐐜𐐝𐐞𐐟")}) //Deseret case sensitive Unicode 6.3 introduced alphabet
  {
    test(u8_to_u32, data.u8, data.u32);
    test(u16_to_u32, data.u16, data.u32);
    test(u32_to_u16, data.u32, data.u16);
    test(u32_to_u8, data.u32, data.u8);
  }

  // Case modifying
  auto u32_uc = [](u32string str) { for (auto&& chr : str) chr = unicode::uppercase(chr); return str; };
  auto u32_lc = [](u32string str) { for (auto&& chr : str) chr = unicode::lowercase(chr); return str; };
  auto u16_uc = [](u16string str) { decltype(str) res; utf16::map(unicode::uppercase, str, res); return res; };
  auto u16_lc = [](u16string str) { decltype(str) res; utf16::map(unicode::lowercase, str, res); return res; };
  auto u8_uc = [](string str) { decltype(str) res; utf8::map(unicode::uppercase, str, res); return res; };
  auto u8_lc = [](string str) { decltype(str) res; utf8::map(unicode::lowercase, str, res); return res; };

  // Case modifying + iterators
  auto u16_uc_str_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf16::decoder(str)) utf16::append(res, unicode::uppercase(chr)); return res; };
  auto u16_uc_buffer_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf16::decoder(str.c_str(), str.size())) utf16::append(res, unicode::uppercase(chr)); return res; };
  auto u8_uc_str_iter = [](string str) { decltype(str) res; for (auto&& chr : utf8::decoder(str)) utf8::append(res, unicode::uppercase(chr)); return res; };
  auto u8_uc_buffer_iter = [](string str) { decltype(str) res; for (auto&& chr : utf8::decoder(str.c_str(), str.size())) utf8::append(res, unicode::uppercase(chr)); return res; };

  // Case modifying tests, iterators tests
  for (auto&& data : {
    make_tuple(U("PřÍlIš žLuŤoUčKý kŮň úPěL ĎáBeLsKé óDy"), U("PŘÍLIŠ ŽLUŤOUČKÝ KŮŇ ÚPĚL ĎÁBELSKÉ ÓDY"), U("příliš žluťoučký kůň úpěl ďábelské ódy")),
    make_tuple(U("В ЧаЩаХ ЮгА ЖиЛ Бы цИтРуС? ДА, нО ФаЛьШиВыЙ ЭкЗеМпЛяРъ!"), U("В ЧАЩАХ ЮГА ЖИЛ БЫ ЦИТРУС? ДА, НО ФАЛЬШИВЫЙ ЭКЗЕМПЛЯРЪ!"), U("в чащах юга жил бы цитрус? да, но фальшивый экземпляръ!")),
    make_tuple(U("𐐨𐐁𐐪𐐃𐐬𐐅𐐮𐐇𐐰𐐉𐐲𐐋𐐴𐐍𐐶𐐏𐐸𐐑𐐺𐐓𐐼𐐕𐐾𐐗𐑀𐐙𐑂𐐛𐑄𐐝𐑆𐐟"), U("𐐀𐐁𐐂𐐃𐐄𐐅𐐆𐐇𐐈𐐉𐐊𐐋𐐌𐐍𐐎𐐏𐐐𐐑𐐒𐐓𐐔𐐕𐐖𐐗𐐘𐐙𐐚𐐛𐐜𐐝𐐞𐐟"), U("𐐨𐐩𐐪𐐫𐐬𐐭𐐮𐐯𐐰𐐱𐐲𐐳𐐴𐐵𐐶𐐷𐐸𐐹𐐺𐐻𐐼𐐽𐐾𐐿𐑀𐑁𐑂𐑃𐑄𐑅𐑆𐑇"))})
  {
    test(u32_uc, get<0>(data).u32, get<1>(data).u32);
    test(u32_lc, get<0>(data).u32, get<2>(data).u32);
    test(u16_uc, get<0>(data).u16, get<1>(data).u16);
    test(u16_lc, get<0>(data).u16, get<2>(data).u16);
    test(u8_uc, get<0>(data).u8, get<1>(data).u8);
    test(u8_lc, get<0>(data).u8, get<2>(data).u8);

    test(u16_uc_str_iter, get<0>(data).u16, get<1>(data).u16);
    test(u16_uc_buffer_iter, get<0>(data).u16, get<1>(data).u16);
    test(u8_uc_str_iter, get<0>(data).u8, get<1>(data).u8);
    test(u8_uc_buffer_iter, get<0>(data).u8, get<1>(data).u8);
  }

  test_summary();

  return 0;
}
