#include "test_data_handler.h"
void TestDataHandler::reload(int cnt) {
  this->cnt = cnt;
  this->pool.clear();
}
bool TestDataHandler::accpet(DemoData data, std::string timestamp) {
  if (this->pool.size() == this->cnt) {
    return true;
  } else {
    this->pool.emplace_back(data, timestamp);
    return this->pool.size() == this->cnt;
  }
}
std::string TestDataHandler::generate() const {
  if (this->pool.size() != this->cnt) return "-1";
  int64_t res = 0;
  for (auto&& [data, timestamp] : this->pool) {
    res += data.diff(data.getHeader().timestamp, timestamp);
  }
  return DataFactory().toString(res / this->cnt);
}
void TestDataHandler::clear() { this->pool.clear(); }

TestDataHandler::TestDataHandler(int cnt) : cnt(cnt) {}
