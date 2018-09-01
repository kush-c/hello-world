#ifndef BRACE_EXPAND_H
#define BRACE_EXPAND_H

#include <string>
#include <vector>

typedef std::vector<std::string> strvector;

class BraceExpand {
private:
  void brace_contents_and_suffix(std::string::const_iterator& itr,
      strvector& contents, strvector& brace_suffixes);
public:
  std::string brace_expand(const std::string& input);
};

#endif
