#include <brace_expand.h>

std::string BraceExpand::brace_expand(const std::string& input) {
  std::vector<std::string> elements;
  std::string curr;
  for(auto itr = input.cbegin(); itr != input.cend(); itr++) {
    switch(*itr) {
      case ',':
      case '{':
      case '}':
        if (curr.size() != 0) {
          elements.push_back(curr);
          curr = "";
        }
        break; // ignore for now
      default:
        curr += *itr;
    }
  }
  elements.push_back(curr);

  std::string result;
  for (int i = 0; i < elements.size(); i++) {
     result += elements[i];
     if (i <= (elements.size() - 2)) {
       result += ",";
     }
   }
   return input + " -> " +result;
}

void BraceExpand::brace_contents_and_suffix(const std::string& input, const int& brace_loc,
    std::vector<std::string>& contents, std::string& brace_suffix) {

}
