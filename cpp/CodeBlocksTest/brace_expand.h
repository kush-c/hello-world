#ifndef BRACE_EXPAND_H
#define BRACE_EXPAND_H

#include <string>
#include <vector>

typedef std::vector<std::string> strvector;
//typedef std::string::const_iterator citr;

class BraceExpand {
private:
  strvector get_elements(const std::string& str, int& loc, const int end, const int stackPos);
  strvector element_or_expansion(const std::string& str, int& loc, const int end, const int stackPos);
  std::string concat(strvector elements);

public:
  std::string brace_expand(const std::string& input);
};

#endif
