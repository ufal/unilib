#pragma once
#include <iostream>
#include <string>
#include <tuple>

using namespace std;

// Simple testing framework
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

void test_summary() {
  cerr << "Passed: " << passed << ", failed: " << failed << endl;
}

// Basic UTF data created during compilation
struct udata {
  string u8;
  u16string u16;
  u32string u32;

  udata(string u8, u16string u16, u32string u32) : u8(u8), u16(u16), u32(u32) {}
};
#define U(str) udata(u8"" str, u"" str, U"" str)
