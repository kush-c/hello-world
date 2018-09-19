#include <brace_expand.h>

namespace {
  const strvector EMPTY_VECTOR_FOR_ERRORS;
}

strvector BraceExpand::brace_expand(const std::string& input) {
  int loc = 0;
  strvector elements = expand_element(input, loc);
  if (loc < input.size()) {
    // only one element (without commas) is permitted at the top level.
    return EMPTY_VECTOR_FOR_ERRORS;
  }
  if (elements.size() == 1 && elements[0].empty()) {
    return EMPTY_VECTOR_FOR_ERRORS;
  }
  return elements;
}

// This method expands a single "element" of the input.
// Conceptually elements are part (or whole) of the input which need to be expanded.
// Elements are separated by commas ocurring at the same level.
// For example in the string "AB,C{D,E}F" -> "AB" and "C{D,E}F" are the elements
// The comma nested between braces does not split "C{D,E}F" into 2 elements as its not at the same "level".
// Elements may be simple eg: "AB" or complex with one or more open/close braces eg: "A{B,C{D,E}}F{G,H}"
// The complex element "A{B,C{D,E}}F{G,H}" has nested elements "B", "C{D,E}", "G" and "H".
// This method returns when it finds a comma indicating the start of next element, or end of input.
// This must be called with the in/out param loc pointing at the start index of the element.
// When it returns the in/out parameter points to the index *after* the end of the lement which may be a comma
// or the end of input.
strvector BraceExpand::expand_element(const std::string& str, int& loc) {
  strvector brace_elements, suffixes;
  std::string prefix;
  bool found_opening_brace = false, found_closing_brace = false;
  while (loc < str.size() && str[loc] != ',') {
    char curr = str[loc];
    if (('a' <= curr && curr <= 'z') || ('A' <= curr && curr <= 'Z')) {
      prefix += curr;
      loc++;
    } else if ('{' == curr) {
      found_opening_brace = true;
      // Get all the elements within the braces.
      brace_elements = get_brace_elements(str, ++loc);
    } else if ('}' == curr) {
      if (!found_opening_brace) {
        // DO not process this closing brace, leave it to the function invocation which encountered the opening brace.
        break;
      }
      found_closing_brace = true;
      // Recursive call expand_element to process the remainder of the element after braces
      // If the suffix itself has expansions eg for element "{a,b}c{d,e}", the recursive call will be responsible
      // for parsing and expanding the suffix "c{d,e}"
      suffixes = expand_element(str, ++loc);
      if (suffixes.empty()) {
        return EMPTY_VECTOR_FOR_ERRORS;
      }
      break;
    } else {
      // Unexpected character
      return EMPTY_VECTOR_FOR_ERRORS;
    }
  }
  if (found_opening_brace && !found_closing_brace) {
    return EMPTY_VECTOR_FOR_ERRORS;
  }
  strvector result;
  // For a complex element say ab{c,d}e{f,g}{i,j}
  // prefix: "ab"
  // brace_elements: "c", "d"
  // suffixes: "efi", "efj", "egi", "egj"
  for (const std::string& elem : brace_elements) {
    for (const std::string& suffix : suffixes) {
      result.push_back(prefix + elem + suffix);
    }
  }
  if (!found_opening_brace) {
    // brace_elements would be empty.
    result.push_back(prefix);
  }
  return result;
}

// Parses and returns all the expanded elements within braces.
// It uses expand_element to parse individual elements
// Eg: This can parse "{AB,B{C,D}}" by using expand_element to parse the individual elements "AB" and "B{C,D}".
// This must be called with the in/out parameter loc pointing to the index of the first char AFTER the opening brace
// Upon returning the in/out parameter loc points to a closing brace.
strvector BraceExpand::get_brace_elements(const std::string& str, int& loc) {
  strvector elements;
  for (;loc < str.size(); loc++) {
    strvector expansion = expand_element(str, loc);
    if (expansion.empty() || (expansion.size() == 1 && expansion[0].empty())) {
      return EMPTY_VECTOR_FOR_ERRORS;
    }
    elements.insert(elements.end(), expansion.begin(), expansion.end());
    if (',' == str[loc]) {
      continue;
     } else if ('}' == str[loc]) {
      break;
    } else {
      return EMPTY_VECTOR_FOR_ERRORS;
    }
  }
  return elements;
}
