#include <iostream>
#include <brace_expand.h>

std::string BraceExpand::brace_expand(const std::string& input) {
  int start_loc = 0;
  strvector elements = get_elements(input, start_loc, input.size());
  return input + " -> " + concat(elements);
}

strvector BraceExpand::get_elements(const std::string& str, int& loc, const int end) {
  const int start_loc = loc;
  std::cout << "GE start_loc: " << start_loc << "\n";
  strvector elements;
  do {
    std::cout << "EoE from GE" << start_loc << "\n";
    strvector expansion = element_or_expansion(str, loc, end);
    elements.insert(elements.end(), expansion.begin(), expansion.end());
  } while (loc < end && str[loc++] == ',');
  std::cout << "GE start_loc: " << start_loc << " loc: " << loc << " elements: " << concat(elements) << "\n";
  return elements;
}

// At least return a vector of a single null string.
strvector BraceExpand::element_or_expansion(const std::string& str, int& loc, const int end) {
  const int start_loc = loc;
  std::cout << "EoE start_loc: " << start_loc << "\n";
  strvector elements;
  strvector suffixes;
  std::string prefix;
  while (loc < end) {
    if (',' == str[loc]) {
      // Finished processing this single element or expansion.
      std::cout << "EoE encountered comma, prefix: " << prefix << ", elements: " << concat(elements) << " suffixes: " << concat(suffixes) << "\n";
      break;
    } else if ('{' == str[loc]) {
      loc++;
      std::cout << "EoE calling GE, prefix " << prefix << ", loc: " << loc << "\n";
      elements = get_elements(str, loc, end);
    } else if ('}' == str[loc]) {
      loc++;
      std::cout << "EoE calling EoE for suffixes, prefix " << prefix << ", loc: " << loc << ", elements: " << concat(elements) << "\n";
      suffixes = element_or_expansion(str, loc, end);
      break;
    } else {
      prefix += str[loc];
      loc++;
      std::cout << "EoE new prefix: " << prefix << ", new loc: " << loc << "\n";
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
  std::cout << "EoE start_loc: " << start_loc << " loc: " << loc << ", prefix: " << prefix
            << ", elements: " << concat(elements) << ", suffixes: " << concat(suffixes) << ", results:" << concat(result) << "\n";
  return result;
}

std::string BraceExpand::concat(strvector elements) {
  std::string result;
  for (size_t i = 0; i < elements.size(); i++) {
     result += elements[i];
     if (i < (elements.size() - 2)) {
       result += "^";
     }
   }
   return result;
}
