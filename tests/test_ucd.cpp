#include <unordered_map>
#include <vector>

#include "test.h"

#include "unicode.h"
using namespace ufal::unilib;

void split(const string& text, char sep, vector<string>& tokens) {
  tokens.clear();
  if (text.empty()) return;

  string::size_type index = 0;
  for (string::size_type next; (next = text.find(sep, index)) != string::npos; index = next + 1)
    tokens.emplace_back(text, index, next - index);
  tokens.emplace_back(text, index);
}

unordered_map<string, unicode::category_t> categories = {
  {"Lu", unicode::Lu}, {"Ll", unicode::Ll}, {"Lt", unicode::Lt}, {"Lm", unicode::Lm}, {"Lo", unicode::Lo},
  {"Mn", unicode::Mn}, {"Mc", unicode::Mc}, {"Me", unicode::Me}, {"Nd", unicode::Nd}, {"Nl", unicode::Nl},
  {"No", unicode::No}, {"Pc", unicode::Pc}, {"Pd", unicode::Pd}, {"Ps", unicode::Ps}, {"Pe", unicode::Pe},
  {"Pi", unicode::Pi}, {"Pf", unicode::Pf}, {"Po", unicode::Po}, {"Sm", unicode::Sm}, {"Sc", unicode::Sc},
  {"Sk", unicode::Sk}, {"So", unicode::So}, {"Zs", unicode::Zs}, {"Zl", unicode::Zl}, {"Zp", unicode::Zp},
  {"Cc", unicode::Cc}, {"Cf", unicode::Cf}, {"Cs", unicode::Cs}, {"Co", unicode::Co}, {"Cn", unicode::Cn},
};

int main(void) {
  // Load categories from UnicodeDate
  string line;
  vector<string> parts;
  while (getline(cin, line)) {
    split(line, ';', parts);
    if (parts.size() != 15) return cerr << "Cannot parse UnicodeData line " << line << endl, 1;

    char32_t code = stoi(parts[0], nullptr, 16);
    test(unicode::category, code, categories[parts[2]]);

    char32_t uppercase = parts[12].empty() ? code : stoi(parts[12], nullptr, 16);
    test(unicode::uppercase, code, uppercase);

    char32_t lowercase = parts[13].empty() ? code : stoi(parts[13], nullptr, 16);
    test(unicode::lowercase, code, lowercase);
  }

  test_summary();

  return 0;
}
