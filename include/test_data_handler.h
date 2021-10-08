#ifndef _TEST_DATA_HANDLER_H
#define _TEST_DATA_HANDLER_H 1
#include <vector>

#include "demo_data.h"
class TestDataHandler {
 private:
  int cnt;
  std::vector<std::pair<DemoData, std::string>> pool;

 public:
  TestDataHandler() = default;
  TestDataHandler(int cnt);
  void reload(int cnt);
  // return true if accumualting enough data.
  bool accpet(DemoData data, std::string timestamp);
  // generate the final output. if not ready ,return "-1"
  std::string generate() const;
  void clear();

  ~TestDataHandler() = default;
};

#endif