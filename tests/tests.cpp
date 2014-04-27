#include <iostream>
#include <string>
#include <tuple>

#include "unicode.h"
#include "utf8.h"
#include "utf16.h"

using namespace ufal::unilib;
using namespace std;

int passed = 0, failed = 0;

template<class Test, class I, class O>
void test(Test test, I input, O output) {
  O result = test(input);
  if (result != output) {
    cerr << "Failed, expected"; for (auto&& chr : output) cerr << ' ' << chr; cerr << ", but got"; for (auto&& chr : result) cerr << ' ' << chr; cerr << "." << endl;
    failed++;
  } else
    passed++;
}

struct udata {
  string u8;
  u16string u16;
  u32string u32;

  udata(string u8, u16string u16, u32string u32) : u8(u8), u16(u16), u32(u32) {}
};
#define U(str) udata(u8"" str, u"" str, U"" str)

int main(void) {
  auto u8_to_u32 = [](string str){ u32string res; utf8::decode(str, res); return res; };
  auto u16_to_u32 = [](u16string str){ u32string res; utf16::decode(str, res); return res; };
  auto u32_to_u16 = [](u32string str){ u16string res; utf16::encode(str, res); return res; };
  auto u32_to_u8 = [](u32string str){ string res; utf8::encode(str, res); return res; };

  for (auto&& data : {
    U("Příliš žluťoučký kůň úpěl ďábelské ódy"),
    U("あめ つち ほし そら / やま かは みね たに / くも きり むろ こけ / ひと いぬ うへ すゑ / ゆわ さる おふ せよ / えのえを なれ ゐて"),
    U("В чащах юга жил бы цитрус? Да, но фальшивый экземпляръ!"),
    U("صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ"),
    U("דג סקרן שט בים מאוכזב ולפתע מצא חברה")})
  {
    test(u8_to_u32, data.u8, data.u32);
    test(u16_to_u32, data.u16, data.u32);
    test(u32_to_u16, data.u32, data.u16);
    test(u32_to_u8, data.u32, data.u8);
  }

  auto u32_uc = [](u32string str) { for (auto&& chr : str) chr = unicode::uppercase(chr); return str; };
  auto u32_lc = [](u32string str) { for (auto&& chr : str) chr = unicode::lowercase(chr); return str; };
  auto u16_uc = [](u16string str) { decltype(str) res; utf16::map(unicode::uppercase, str, res); return res; };
  auto u16_lc = [](u16string str) { decltype(str) res; utf16::map(unicode::lowercase, str, res); return res; };
  auto u8_uc = [](string str) { decltype(str) res; utf8::map(unicode::uppercase, str, res); return res; };
  auto u8_lc = [](string str) { decltype(str) res; utf8::map(unicode::lowercase, str, res); return res; };

  auto u16_uc_str_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf16::decoder(str)) utf16::append(res, unicode::uppercase(chr)); return res; };
  auto u16_uc_buffer_iter = [](u16string str) { decltype(str) res; for (auto&& chr : utf16::decoder(str.c_str(), str.size())) utf16::append(res, unicode::uppercase(chr)); return res; };
  auto u8_uc_str_iter = [](string str) { decltype(str) res; for (auto&& chr : utf8::decoder(str)) utf8::append(res, unicode::uppercase(chr)); return res; };
  auto u8_uc_buffer_iter = [](string str) { decltype(str) res; for (auto&& chr : utf8::decoder(str.c_str(), str.size())) utf8::append(res, unicode::uppercase(chr)); return res; };

  for (auto&& data : {
    make_tuple(U("PřÍlIš žLuŤoUčKý kŮň úPěL ĎáBeLsKé óDy"), U("PŘÍLIŠ ŽLUŤOUČKÝ KŮŇ ÚPĚL ĎÁBELSKÉ ÓDY"), U("příliš žluťoučký kůň úpěl ďábelské ódy")),
    make_tuple(U("В ЧаЩаХ ЮгА ЖиЛ Бы цИтРуС? ДА, нО ФаЛьШиВыЙ ЭкЗеМпЛяРъ!"), U("В ЧАЩАХ ЮГА ЖИЛ БЫ ЦИТРУС? ДА, НО ФАЛЬШИВЫЙ ЭКЗЕМПЛЯРЪ!"), U("в чащах юга жил бы цитрус? да, но фальшивый экземпляръ!"))})
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

  cerr << "Passed: " << passed << ", failed: " << failed << endl;

  return 0;
}
