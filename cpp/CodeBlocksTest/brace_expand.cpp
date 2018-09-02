#include <iostream>
#include <brace_expand.h>

std::string BraceExpand::brace_expand(const std::string& input) {
  strvector elements = get_elements(input.cbegin(), input.cend());
  std::string result;
  for (size_t i = 0; i < elements.size(); i++) {
     result += elements[i];
     if (i < (elements.size() - 2)) {
       result += "^";
     }
   }
   return input + " -> " +result;
}

const strvector BraceExpand::get_elements(citr& itr, citr end) {
  strvector elements;
  strvector suffixes;
  std::string curr;

  for(;itr != end; itr++) {
    if (*itr == ',') {
      if (curr.size() != 0) {
        elements.push_back(curr);
        curr = "";
      }
    } else if (*itr == '{') { // nested brace, we will ensure we process the closing brace internally.
      strvector contents = brace_contents(++itr, input.cend());
      for (const std::string& content : contents) {
        elements.push_back(curr + content);
      }
      curr = "";
    } else if (*itr == '}') {
      suffixes = suffix_contents(++itr, end);
      break;
    } else {
      curr += *itr;
    }
  }

  strvector results;
  for (const std::string& elem : elements) {
    for (const std::string& suffix : suffixes) {
      results.push_back(elem + suffix);
    }
  }
  return results;
}

const strvector BraceExpand::element_or_expansion(citr& itr, citr end) {
  strvector result;
  std::string prefix;
  while(itr != end) {
    if (',' == *itr) {
      itr++;
      result.push_back(prefix);
      break;
    } else if ('{' == *itr) {
      itr++;
      const strvector elements = get_elements(itr, end);
      for (const std::string& elem : elements) {
        result.push_back(prefix + elem);
      }
      break;
    } else if ('}' == *itr) {
      // We have to handle the Suffixes here.
      // HOW DO WE DO THAT??
    }
  }
  return result;
}
