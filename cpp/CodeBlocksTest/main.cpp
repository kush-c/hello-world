#include <iostream>
#include <brace_expand.h>

int main(int argc, char const *argv[]) {
  strvector cases = {
    "{A,B}",
    //"AB,C",
    //"{A,B}{C,D}",
    //"{A,B{C,D}}",
    //"{ABC}",
    //"ABC"
  };
  BraceExpand b;
  //std::cout << b.brace_expand("Hello, Ishu baby!!") << "\n";
  for (const auto& str : cases) {
    std::cout << b.brace_expand(str) << "\n";
  }
  return 0;
}
