#ifndef BRACE_EXPAND_H
#define BRACE_EXPAND_H

#include <string>
#include <vector>

typedef std::vector<std::string> strvector;
typedef std::string::const_iterator citr;

class BraceExpand {
private:
  const strvector get_elements(citr& itr, citr end);
  const strvector element_or_expansion(citr& itr, citr end);

public:
  std::string brace_expand(const std::string& input);
};

#endif
