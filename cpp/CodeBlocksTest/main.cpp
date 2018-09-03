#include <iostream>
#include <brace_expand.h>

int main(int argc, char const *argv[]) {
  strvector cases = {
    "{A,B}",
    "AB,C",
    "AB,A{B,C{X,YZ}}D{E,F}",
    "A{B,C{X,YZ}}D{E,F}",
    "{A,B}{C,D}",
    "{A,B{C,D}}",
    "{ABC}",
    "ABC",
    "}ABC",
    "X{ABC",
    "{A,B,}",
    "}{",
    "{}",
    "A,B,C",
    "{A{B,C}",
    "",
    "A#B",
    "A{B,C}D#"
  };
  BraceExpand b;
  for (const auto& str : cases) {
    std::cout << str << " -> ";
    strvector expansion = b.brace_expand(str);
    if (expansion.empty()) std::cout << "invalid input\n";
    for (int i = 0; i < expansion.size(); ++i) {
      const char suffix = i < (expansion.size() - 1) ? ' ' : '\n';
      std::cout << expansion[i] << suffix;
    }
  }
  return 0;
}
