#include <brace_expand.h>

std::string BraceExpand::brace_expand(const std::string& input) {
  std::vector<std::string> elements;
  std::string curr;
  for(auto itr = input.cbegin(); itr != input.cend(); itr++) {
    switch(*itr) {
      case ',':
        elements.push_back(curr);
        curr = "";
        break;
      case '{':
      case '}':
        break; // ignore for now
      default:
        curr += *itr;
    }
  }

  std::string result;
  for (int i = 0; i < elements.size(); i++) {
     result += elements[i];
     if (i != elements.size() - 1) {
       result += " ";
     }
   }
   return result;
}

void BraceExpand::brace_contents_and_suffix(const std::string& input, const int& brace_loc,
    std::vector<std::string>& contents, std::string& brace_suffix) {

}
