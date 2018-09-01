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
        strvector brace_contents;
        strvector brace_suffixes;
        brace_contents_and_suffix(itr, brace_contents, brace_suffixes);
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

void BraceExpand::brace_contents_and_suffix(std::string::const_iterator& itr,
    strvector& contents, strvector& brace_suffixes) {

}
