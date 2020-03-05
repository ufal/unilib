#include <algorithm>
#include <locale>
#include <regex>
#include <string>

#include "unicode.h"
#include "unistrip.h"

namespace ufal {
namespace unilib {

class uniregex_traits {
 public:
  using char_type = char32_t;
  using string_type = std::u32string;
  using locale_type = std::locale;
  using char_class_type = unicode::category_t;

  uniregex_traits() : locale(std::locale::classic(), &ctype) {}

  static std::size_t length(const char32_t* str) {
    return std::u32string::traits_type::length(str);
  }

  char32_t translate(char32_t chr) const {
    return chr;
  }

  char32_t translate_nocase(char32_t chr) const {
    return unicode::lowercase(chr);
  }

  template<class ForwardIt>
  std::u32string transform(ForwardIt first, ForwardIt last) const {
    return std::u32string(first, last);
  }

  template<class ForwardIt>
  std::u32string transform_primary(ForwardIt first, ForwardIt last) const {
    std::u32string result;
    for (; first != last; first++) {
      const auto& chr = *first;
      if (!unistrip::is_combining_mark(chr))
        result.push_back(unicode::lowercase(unistrip::strip_combining_marks(chr)));
    }
    return result;
  }

  template<class ForwardIt>
  std::u32string lookup_collatename(ForwardIt first, ForwardIt last) const {
    return std::u32string(first, last);
  }

  template<class ForwardIt>
  unicode::category_t lookup_classname(ForwardIt first, ForwardIt last, bool icase=false) const {
    std::u32string cls(first, last);
    if (icase && cls.compare(0, 1, U"L")) return unicode::L;
    if (cls == U"Lu") return unicode::Lu;
    if (cls == U"Ll") return unicode::Ll;
    if (cls == U"Lt") return unicode::Lt;
    if (cls == U"Lut") return unicode::Lut;
    if (cls == U"LC") return unicode::LC;
    if (cls == U"Lm") return unicode::Lm;
    if (cls == U"Lo") return unicode::Lo;
    if (cls == U"L") return unicode::L;

    if (cls == U"Mn") return unicode::Mn;
    if (cls == U"Mc") return unicode::Mc;
    if (cls == U"Me") return unicode::Me;
    if (cls == U"M") return unicode::M;

    if (cls == U"Nd") return unicode::Nd;
    if (cls == U"Nl") return unicode::Nl;
    if (cls == U"No") return unicode::No;
    if (cls == U"N") return unicode::N;

    if (cls == U"Pc") return unicode::Pc;
    if (cls == U"Pd") return unicode::Pd;
    if (cls == U"Ps") return unicode::Ps;
    if (cls == U"Pe") return unicode::Pe;
    if (cls == U"Pi") return unicode::Pi;
    if (cls == U"Pf") return unicode::Pf;
    if (cls == U"Po") return unicode::Po;
    if (cls == U"P") return unicode::P;

    if (cls == U"Sm") return unicode::Sm;
    if (cls == U"Sc") return unicode::Sc;
    if (cls == U"Sk") return unicode::Sk;
    if (cls == U"So") return unicode::So;
    if (cls == U"S") return unicode::S;

    if (cls == U"Zs") return unicode::Zs;
    if (cls == U"Zl") return unicode::Zl;
    if (cls == U"Zp") return unicode::Zp;
    if (cls == U"Z") return unicode::Z;

    if (cls == U"Cc") return unicode::Cc;
    if (cls == U"Cf") return unicode::Cf;
    if (cls == U"Cs") return unicode::Cs;
    if (cls == U"Co") return unicode::Co;
    if (cls == U"Cn") return unicode::Cn;
    if (cls == U"C") return unicode::C;

    return 0;
  }

  bool isctype(char32_t chr, unicode::category_t categories) const {
    return unicode::category(chr) & categories;
  }

  int value(char32_t chr, int radix) const {
    if (radix == 8 && chr >= U'0' && chr <= U'7') return chr - U'0';
    if ((radix == 10 || radix == 16) && chr >= U'0' && chr <= U'9') return chr - U'0';
    if (radix == 16 && chr >= U'A' && chr <= U'F') return chr - U'A'+ 10;
    if (radix == 16 && chr >= U'a' && chr <= U'f') return chr - U'a'+ 10;
    return -1;
  }

  locale_type imbue(locale_type locale) {
    std::swap(this->locale, locale);
    return locale;
  }

  locale_type getloc() const {
    return locale;
  }

 private:
  class unictype : public std::ctype<char32_t> {
   public:
  };
  unictype ctype;

  locale_type locale;
};

using uniregex = std::basic_regex<char32_t, uniregex_traits>;
using uniregex_iterator = std::regex_iterator<std::u32string::const_iterator, char32_t, uniregex_traits>;
using unisub_match = std::sub_match<std::u32string::const_iterator>;
using unimatch_results = std::match_results<std::u32string::const_iterator>;

} // namespace unilib
} // namespace ufal

#include <iostream>
#include <memory>

#include "utf8.h"

using namespace std;

int main() {
  string line;
  unique_ptr<ufal::unilib::uniregex> regex;
  while (getline(cin, line)) {
    if (!line.size()) {
      regex.reset();
      continue;
    }

    u32string u32line;
    ufal::unilib::utf8::decode(line, u32line);

    if (!regex) {
      regex.reset(new ufal::unilib::uniregex(u32line));
    } else {
      for (auto&& it = ufal::unilib::uniregex_iterator(u32line.begin(), u32line.end(), *regex); it != ufal::unilib::uniregex_iterator(); it++) {
        auto&& match = *it;

        string result;
        ufal::unilib::utf8::encode(match.str(), result);
        cout << result << endl;
      }
    }
  }

  return 0;
}

