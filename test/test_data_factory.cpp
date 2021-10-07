#include <string>
#include <vector>

#include "data_factory.h"
#include "test_framework.h"
std::vector<std::pair<std::string, bool>> getIntDataset() {
  std::vector<std::pair<std::string, bool>> res;
  res.emplace_back("1234", true);
  res.emplace_back("123@4", false);
  res.emplace_back("@41", false);
  return res;
}
int main(int argc, char const *argv[]) {
  DataFactory factory;
  for (auto [str, result] : getIntDataset()) {
    DemoTest::assertEqual(factory.stringTo<int>(str).first, result);
  }
  return 0;
}
