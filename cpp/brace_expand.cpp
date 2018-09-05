#include <brace_expand.h>

namespace {
  const strvector EMPTY_VECTOR_FOR_ERRORS;
}

// Expands the braces of the string by imitating a state machine, which processes parts (or elements)
// within the input string.
// Elements are parts of the input which may be partitioned by the comma *OUTSIDE* braces.
// For Eg: "ab,cd" has 2 elements "ab" and "cd" but "a{b,c}" is considered a single complex element as the comma occurs within braces.
// The state machine takes turns by enumerating the elements within braces, and then parsing and expanding individual elements
// The "top-level" input string is considered to be a single complex element as no commas are allowed outside braces.
strvector BraceExpand::brace_expand(const std::string& input) {
  int start_loc = 0;
  strvector elements = element_or_expansion(input, start_loc, /*outside_braces=*/true);
  if (elements.size() == 1 && elements[0].empty()) {
    return EMPTY_VECTOR_FOR_ERRORS;
  }
  return elements;
}

// Expands a single element which may be simple or complex.
// Simple elements such as "AC" expand to themselves.
// This must be called with the in/out parameter loc being the index of the first char of the element to be parsed.
// After this is done processing the in/out parameter loc points at:
// 1: a comma,
// 2: an ending brace which is not part of the current element, but part of a higher level complex element
// or 3: end of string
strvector BraceExpand::element_or_expansion(const std::string& str, int& loc, const bool outside_braces) {
  strvector brace_elements, suffixes;
  std::string prefix;
  bool found_opening_brace = false, found_closing_brace = false;
  while (loc < str.size()) {
    char curr = str[loc];
    if (('a' <= curr && curr <= 'z') || ('A' <= curr && curr <= 'Z')) {
      prefix += curr;
      loc++;
    } else if (',' == curr) {
      if (outside_braces) {
        // commas invalid outside braces
        return EMPTY_VECTOR_FOR_ERRORS;
      }
      // Finished processing this element.
      break;
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
      // Recursive call element_or_expansion to process the remainder of the element after braces
      // If the suffix itself has expansions eg for element "{a,b}c{d,e}", the recursive call will be responsible
      // for parsing and expanding the suffix "c{d,e}"
      suffixes = element_or_expansion(str, ++loc, outside_braces);
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
// It uses element_or_expansion to parse individual elements
// Eg: This can parse {"AB", B{C,D"}} by using element_or_expansion to parse the individual
// elements "AB" and "B{C,D}".
// This must be called with the in/out parameter loc pointing to the index of the first char AFTER the opening brace
// Upon returning the in/out parameter loc points to a closing brace.
strvector BraceExpand::get_brace_elements(const std::string& str, int& loc) {
  strvector elements;
  for (;loc < str.size(); loc++) {
    strvector expansion = element_or_expansion(str, loc, /*outside_braces=*/false);
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
