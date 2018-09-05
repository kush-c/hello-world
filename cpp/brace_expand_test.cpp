#include <iostream>
#include <brace_expand.h>

int main(int argc, char const *argv[]) {
  std::vector<strvector> cases = {
    {"{A,b}{d,E}", "Ad AE bd bE "},
    {"{A,B}", "A B "},
    {"a{b,c}d", "abd acd "},
    {"aB{{c,d},{c,d}e}f", "aBcf aBdf aBcef aBdef "},
    {"A{,c,d}B",},
    {"A{c,d}B", "AcB AdB "},
    {"A,{c,d}B"},
    {"A{c,d},B"},
    {"A{c}B", "AcB "},
    {"{{{{c,d}}e}}", "ce de "},
    {"{ab,cd{e,f}}", "ab cde cdf "},
    {"{}ab"},
    {"AB,C"},
    {"AB,A{B,C{X,YZ}}D{E,F}"},
    {"A{B,C{X,YZ}}D{E,F}", "ABDE ABDF ACXDE ACXDF ACYZDE ACYZDF "},
    {"{A,B}{C,D}", "AC AD BC BD "},
    {"{A,B{C,D}}", "A BC BD "},
    {"{ABC}", "ABC "},
    {"ABC", "ABC "},
    {"}ABC"},
    {"X{ABC"},
    {"{A,B,}"},
    {"}{"},
    {"{}"},
    {"A,B,C"},
    {"{A{B,C}"},
    {""},
    {"A#B"},
    {"A{B,C}D#"},
    {"abc{c,d,e}}"},
  };
  int pass = 0, fail = 0;
  BraceExpand b;
  for (const strvector& test_case : cases) {
    std::string expected = test_case.size() == 2 ? test_case[1] : "%invalid%";
    std::string actual;

    strvector expansion = b.brace_expand(test_case[0]);
    if (expansion.empty()) {
      actual = "%invalid%";
    } else {
      for (const std::string& element : expansion) actual += (element + " ");
    }
    std::cout << "Test case: " << test_case[0] << " -> " << expected;
    if (expected == actual) {
      pass++;
      std::cout << " : SUCCESS\n";
    } else {
      fail++;
      std::cout << " : FAIL Got Result: " << actual << "\n";
    }
  }
  std::cout << "Finished Testing: " << pass << " Passed, " << fail << " Failed.\n";
  return fail == 0 ? 0 : 1;
}
