#include <iostream>
#include <vector>
#include <brace_expand.h>


int main(int argc, char const *argv[]) {
  std::vector<std::string> cases = {"{A,B,C}", "{A,B}{C,D}", "{A,B{C,D}}", "{ABC}", "ABC"};
  BraceExpand b;
  for (auto str : cases) {
    std::cout << b.brace_expand(str) << "\n";
  }
  return 0;
}
