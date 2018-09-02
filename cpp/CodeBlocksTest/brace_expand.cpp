#include <iostream>
#include <brace_expand.h>

std::string BraceExpand::brace_expand(const std::string& input) {
  strvector elements;
  std::string curr;
  for(auto itr = input.cbegin(); itr != input.cend(); itr++) {
    switch(*itr) {
      case ',':
        if (curr.size() != 0) {
          elements.push_back(curr);
          curr = "";
        }
        break;
      case '{':
      {
        strvector contents = brace_contents(++itr, input.cend());
        for (const std::string& content : contents) {
          elements.push_back(curr + content);
        }
        curr = "";
        break;
      }
      case '}':
        std::cout << "THIS IS UNEXPECTED\n";
        break;
      default:
        curr += *itr;
    }
  }
  elements.push_back(curr);

  std::string result;
  for (size_t i = 0; i < elements.size(); i++) {
     result += elements[i];
     if (i < (elements.size() - 2)) {
       result += "^";
     }
   }
   return input + " -> " +result;
}

strvector BraceExpand::brace_contents(citr& itr, citr end) {
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

strvector BraceExpand::suffix_contents(citr& itr, citr end) {
  strvector suffixes;
  if (itr == end) {
    return suffixes;
  }

  return suffixes;
}
