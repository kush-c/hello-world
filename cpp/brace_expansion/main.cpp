#include <iostream>
#include <brace_expand.h>

int main(int argc, char const *argv[]) {
  std::string input;
  std::cin >> input;
  BraceExpand b;
  strvector expansion = b.brace_expand(input);
  for (size_t i = 0; i < expansion.size(); ++i) {
    const char suffix = i < (expansion.size() - 1) ? ' ' : '\n';
    std::cout << expansion[i] << suffix;
  }
  return 0;
}
