#ifndef BRACE_EXPAND_H
#define BRACE_EXPAND_H

#include <string>
#include <vector>

typedef std::vector<std::string> strvector;

class BraceExpand {
private:
  strvector element_or_expansion(const std::string& str, int& loc, const int end, bool outer_element);
  strvector get_elements(const std::string& str, int& loc, const int end);
public:
  strvector brace_expand(const std::string& input);
};

#endif
