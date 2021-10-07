#ifndef _DEMO_DATA_H
#define _DEMO_DATA_H 1
#include <chrono>
#include <sstream>
#include <string>

#include "data_factory.h"
enum DataType {
  data_invalid,
  session_init,
  session_pair,
  delivery_data,
  conn_close,
};
struct DataHeader {
  DataType type;
  std::string timestamp;
};

struct DataBody {
  std::string content;
};
class DemoData {
 private:
  typedef std::chrono::time_point<std::chrono::steady_clock,
                                  std::chrono::microseconds>
      MicroClockType;
  DataHeader header;
  DataBody body;
  static const DataFactory factory;

 public:
  DemoData();
  DemoData(DataType type);
  DemoData(DataType type, std::string content);
  DemoData(DataType type, std::string timestamp, std::string content);
  /// Layout: type(1 char) nTimestamp(int64_t) timestamp() content
  DemoData(std::string source);
  // microsecond: since epoch
  static int64_t diff(std::string begin, std::string end) {
    auto [endFlag, endResult] = factory.stringTo<int64_t>(end);
    auto [beginFlag, beginResult] = factory.stringTo<int64_t>(begin);
    return endFlag && beginFlag ? endResult - beginResult : -1;
  }
  // microsecond: since epoch
  static std::string now() {
    MicroClockType cur =
        std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now());
    return factory.toString<int64_t>(cur.time_since_epoch().count());
  }
  std::string toStr() const;
  long long getSize() const;
  bool isNull() const;
  DataHeader getHeader() const;
  DataBody getBody() const;
};
#endif  // demo_data.h