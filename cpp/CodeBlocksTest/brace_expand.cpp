#include <brace_expand.h>

strvector BraceExpand::brace_expand(const std::string& input) {
  int start_loc = 0;
  strvector elements = element_or_expansion(input, start_loc, input.size(), /*outside_braces=*/true);
  if (elements.size() == 1 && elements[0].empty()) {
    return strvector();
  }
  return elements;
}

strvector BraceExpand::element_or_expansion(const std::string& str, int& loc, const int end, bool outside_braces) {
  strvector elements, suffixes;
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
        return strvector();
      }
      // Finished processing this element.
      break;
    } else if ('{' == curr) {
      found_opening_brace = true;
      elements = get_elements(str, ++loc, end);
    } else if ('}' == curr) {
      if (!found_opening_brace) {
        // DO not process this closing brace, leave it to the function invocation which encountered the opening brace.
        break;
      }
      found_closing_brace = true;
      suffixes = element_or_expansion(str, ++loc, end, outside_braces);
      if (suffixes.empty()) {
        // Invalid suffix found.
        return strvector();
      }
      break;
    } else {
      // Unexpected character
      return strvector();
    }
  }
  if (found_opening_brace && !found_closing_brace) {
    return strvector();
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
  return result;
}

strvector BraceExpand::get_elements(const std::string& str, int& loc, const int end) {
  strvector elements;
  for (;loc < end; loc++) {
    strvector expansion = element_or_expansion(str, loc, end, /*outside_braces=*/false);
    if (expansion.empty() || (expansion.size() == 1 && expansion[0].empty())) {
      // Invalid segment encountered.
      return strvector();
    }
    elements.insert(elements.end(), expansion.begin(), expansion.end());
    if (',' == str[loc]) {
      continue;
     } else if ('}' == str[loc]) {
      break;
    } else {
      return strvector();
    }
  }
  return elements;
}
