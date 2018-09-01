#include <brace_expand.h>

std::string BraceExpand::brace_expand(std::string input) {
  return "real " + input;
}

/*
void brace_expand(std::string& str) {
  std::cout << str << "13\n";
}

int main(int argc, char const *argv[]) {
  std::vector<std::string> cases = {"{A,B,C}", "{A,B}{C,D}", "{A,B{C,D}}", "{ABC}", "ABC"};
  for (auto str : cases) {
    brace_expand(str);
  }
  return 0;
}
*/
