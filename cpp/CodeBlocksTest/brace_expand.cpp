#include <iostream>
#include <brace_expand.h>

strvector BraceExpand::brace_expand(const std::string& input) {
  int start_loc = 0;
  strvector elements = element_or_expansion(input, start_loc, input.size(), true, 1);
  std::cout << input + " -> " + concat(elements) + "\n";
  return elements;
}

strvector BraceExpand::get_elements(const std::string& str, int& loc, const int end, const int stackPos) {
  const int start_loc = loc;
  std::cout << "GE start_loc: " << start_loc <<  ", end: " << end << ", stackPos: " << stackPos <<  "\n";
  strvector elements;
  do {
    strvector expansion = element_or_expansion(str, loc, end, false, stackPos + 1);
    if (expansion.empty()) {
      // Invalid segment encountered.
      return strvector();
    }
    elements.insert(elements.end(), expansion.begin(), expansion.end());
    std::cout << "GE Current loc: " << loc << ", current elements: " << concat(elements) << "\n";
    char curr = str[loc];
    if (',' == curr) {
      loc++;
    } else if ('}' == curr) {
      break;
    } else {
      // Unexpected character encountered.
      return strvector();
    }
  } while (loc < end);
  std::cout << "GE finished start_loc: " << start_loc << " loc: " << loc << ", stackPos: " << stackPos << " elements: " << concat(elements) << "\n";
  return elements;
}

// At least return a vector of a single null string.
strvector BraceExpand::element_or_expansion(const std::string& str, int& loc, const int end, bool outside_braces, const int stackPos) {
  const int start_loc = loc;
  std::cout << "EoE start_loc: " << start_loc << ", stackPos: " << stackPos << "\n";
  strvector elements;
  strvector suffixes;
  std::string prefix;
  bool found_opening_brace = false;
  while (loc < end) {
    char curr = str[loc];
    if (('a' <= curr && curr <= 'z') || ('A' <= curr && curr <= 'Z')) {
      prefix += curr;
      loc++;
    } else if (',' == curr) {
      if (outside_braces) {
        // commas invalid outside braces
        return strvector();
      }
      // Finished processing this single element or expansion.
      std::cout << "EoE encountered comma, prefix: " << prefix << ", elements: " << concat(elements) << " suffixes: " << concat(suffixes) << "\n";
      break;
    } else if ('{' == curr) {
      found_opening_brace = true;
      loc++;
      elements = get_elements(str, loc, end, stackPos + 1);
    } else if ('}' == curr) {
      if (!found_opening_brace) {
        // DO not process this closing brace, leave it to the function invocation which encountered the opening brace.
        break;
      }
      loc++;
      std::cout << "EoE calling EoE for suffixes, prefix " << prefix << ", loc: " << loc << ", stackPos: " << stackPos << ", elements: " << concat(elements) << "\n";
      suffixes = element_or_expansion(str, loc, end, outside_braces, stackPos + 1);
      break;
    } else {
      // Unexpected character
      return strvector();
    }
  }
  strvector result;
  for (const std::string& elem : elements) {
    for (const std::string& suffix : suffixes) {
      result.push_back(prefix + elem + suffix);
    }
  }
  if (result.empty()) {
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
