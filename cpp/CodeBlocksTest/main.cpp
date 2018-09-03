#include <iostream>
#include <brace_expand.h>

int main(int argc, char const *argv[]) {
  if (argc < 2 ) {
    return 1;
  }
  BraceExpand b;
  strvector expansion = b.brace_expand(argv[1]);
  for (size_t i = 0; i < expansion.size(); ++i) {
    const char suffix = i < (expansion.size() - 1) ? ' ' : '\n';
    std::cout << expansion[i] << suffix;
  }
  return 0;
}
