#include <iostream>
#include <brace_expand.h>

std::string BraceExpand::brace_expand(const std::string& input) {
  int start_loc = 0;
  strvector elements = get_elements(input, start_loc, input.size());
  std::string result;
  for (size_t i = 0; i < elements.size(); i++) {
     result += elements[i];
     if (i < (elements.size() - 2)) {
       result += "^";
     }
   }
   return input + " -> " +result;
}

strvector BraceExpand::get_elements(const std::string& str, int& loc, const int end) {
  strvector v;
  return v;
}

// At least return a vector of a single null string.
strvector BraceExpand::element_or_expansion(const std::string& str, int& loc, const int end) {
  strvector elements;
  strvector suffixes;
  std::string prefix;
  while(loc != end) {
    if (',' == str[loc]) {
      loc++;
      break;
    } else if ('{' == str[loc]) {
      loc++;
      elements = get_elements(str, loc, end);
    } else if ('}' == str[loc]) {
      loc++;
      suffixes = element_or_expansion(str, loc, end);
      break;
    } else {
      prefix += str[loc];
      loc++;
    }
  }
  strvector result;
  for (const std::string& elem : elements) {
    for (const std::string& suffix : suffixes) {
      result.push_back(prefix + elem + suffix);
    }
  }
  if (result.size() == 0) {
    result.push_back(prefix);
  }
  return result;
}
