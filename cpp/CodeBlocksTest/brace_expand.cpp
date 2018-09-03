#include <brace_expand.h>

namespace {
  const strvector EMPTY_VECTOR_FOR_ERRORS;
}

strvector BraceExpand::brace_expand(const std::string& input) {
  int start_loc = 0;
  strvector elements = element_or_expansion(input, start_loc, input.size(), /*outside_braces=*/true);
  if (elements.size() == 1 && elements[0].empty()) {
    return EMPTY_VECTOR_FOR_ERRORS;
  }
  return elements;
}

strvector BraceExpand::element_or_expansion(const std::string& str, int& loc, const int end, const bool outside_braces) {
  strvector brace_elements, suffixes;
  std::string prefix;
  bool found_opening_brace = false, found_closing_brace = false;
  while (loc < end) {
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
      brace_elements = get_brace_elements(str, ++loc, end);
    } else if ('}' == curr) {
      if (!found_opening_brace) {
        // DO not process this closing brace, leave it to the function invocation which encountered the opening brace.
        break;
      }
      found_closing_brace = true;
      suffixes = element_or_expansion(str, ++loc, end, outside_braces);
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

strvector BraceExpand::get_brace_elements(const std::string& str, int& loc, const int end) {
  strvector elements;
  for (;loc < end; loc++) {
    strvector expansion = element_or_expansion(str, loc, end, /*outside_braces=*/false);
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
