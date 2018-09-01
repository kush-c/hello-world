#include <iostream>
#include <brace_expand.h>

int main(int argc, char const *argv[]) {
  BraceExpand b;
  std::cout << b.brace_expand("Hello, Ishu baby!!") << "\n";
  return 0;
}
