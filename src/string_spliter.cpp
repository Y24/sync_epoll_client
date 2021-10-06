#include "string_spliter.h"
std::vector<std::string> StringSpliter::split(std::string text,
                                              std::string seperator) {
  std::regex ws_re(seperator);
  std::vector<std::string> v(
      std::sregex_token_iterator(text.begin(), text.end(), ws_re, -1),
      std::sregex_token_iterator());
  return v;
}