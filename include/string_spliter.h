#ifndef _STRING_SPLITER_H
#define _STRING_SPLITER_H 1
#include <regex>
#include <string>
#include <vector>
class StringSpliter {
 public:
  StringSpliter() = default;
  std::vector<std::string> split(std::string text,
                                 std::string seperator = "\\s+");
  ~StringSpliter() = default;
};
#endif  // string_spliter.h
