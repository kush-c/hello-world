#ifndef BRACE_EXPAND_H
#define BRACE_EXPAND_H

#include <string>
#include <vector>

class BraceExpand {
private:
  void brace_contents_and_suffix(const std::string& input, const int& brace_loc,
      std::vector<std::string>& contents, std::string& brace_suffix);
public:
  std::string brace_expand(const std::string& input);
};

#endif
