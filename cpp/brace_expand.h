#ifndef BRACE_EXPAND_H
#define BRACE_EXPAND_H

#include <string>
#include <vector>

typedef std::vector<std::string> strvector;

class BraceExpand {
private:
  strvector expand_element(const std::string& str, int& loc, const bool outside_braces);
  strvector get_brace_elements(const std::string& str, int& loc);
public:
  // Expands the braces in the string and returns the expanded elements in a vector.
  strvector brace_expand(const std::string& input);
};

#endif
