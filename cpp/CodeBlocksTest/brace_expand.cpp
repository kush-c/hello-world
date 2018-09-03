#include <iostream>
#include <brace_expand.h>

std::string BraceExpand::brace_expand(const std::string& input) {
  int start_loc = 0;
  strvector elements = get_elements(input, start_loc, input.size(), 1);
  return input + " -> " + concat(elements);
}

strvector BraceExpand::get_elements(const std::string& str, int& loc, const int end, const int stackPos) {
  const int start_loc = loc;
  std::cout << "GE start_loc: " << start_loc <<  ", end: " << end << ", stackPos: " << stackPos <<  "\n";
  strvector elements;
  while(true) {
    strvector expansion = element_or_expansion(str, loc, end, stackPos + 1);
    elements.insert(elements.end(), expansion.begin(), expansion.end());
    std::cout << "GE Current loc: " << loc << ", current elements: " << concat(elements) << "\n";
    // This loop could be more elegant.
    if (loc < end && str[loc] == ',') {
      loc++;
    } else {
      break;
    }
  }
  std::cout << "GE finished start_loc: " << start_loc << " loc: " << loc << ", stackPos: " << stackPos << " elements: " << concat(elements) << "\n";
  return elements;
}

// At least return a vector of a single null string.
strvector BraceExpand::element_or_expansion(const std::string& str, int& loc, const int end, const int stackPos) {
  const int start_loc = loc;
  std::cout << "EoE start_loc: " << start_loc << ", stackPos: " << stackPos << "\n";
  strvector elements;
  strvector suffixes;
  std::string prefix;
  bool found_opening_brace = false;
  while (loc < end) {
    if (',' == str[loc]) {
      // Finished processing this single element or expansion.
      std::cout << "EoE encountered comma, prefix: " << prefix << ", elements: " << concat(elements) << " suffixes: " << concat(suffixes) << "\n";
      break;
    } else if ('{' == str[loc]) {
      found_opening_brace = true;
      loc++;
      //std::cout << "EoE calling GE, prefix " << prefix << ", loc: " << loc << "\n";
      elements = get_elements(str, loc, end, stackPos + 1);
    } else if ('}' == str[loc]) {
      if (found_opening_brace) {
        loc++;
        std::cout << "EoE calling EoE for suffixes, prefix " << prefix << ", loc: " << loc << ", stackPos: " << stackPos << ", elements: " << concat(elements) << "\n";
        suffixes = element_or_expansion(str, loc, end, stackPos + 1);
      } else {
        // DO not process this closing brace.
      }
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
  std::cout << "EoE finished start_loc: " << start_loc << " loc: " << loc << ", prefix: " << prefix << ", stackPos: " << stackPos
            << ", elements: " << concat(elements) << ", suffixes: " << concat(suffixes) << ", results:" << concat(result) << "\n";
  return result;
}

std::string BraceExpand::concat(strvector elements) {
  if (elements.size() == 0) {
    return "empty";
  }
  std::string result;
  for (size_t i = 0; i < elements.size(); i++) {
     result += elements[i];
     //if (i < (elements.size() - 2)) {
       result += "+";
     //}
   }
   return result;
}
