#ifndef _DATA_FACTORY_H
#define _DATA_FACTORY_H 1
class DataFactory {
 public:
  DataFactory() = default;
  ~DataFactory() = default;
  template <class T>
  static T charTo(char c) {
    return (T)c;
  }
  template <class T>
  static char toChar(T t) {
    return (char)t;
  }
  template <class T>
  static T stringTo(std::string s) {
    T res;
    std::istringstream is(s);
    is >> res;
    return res;
  }
  template <class T>
  static std::string toString(T t) {
    std::ostringstream os;
    os << t;
    std::string res;
    std::istringstream is(os.str());
    is >> res;
    return res;
  }
};
#endif  // data_factory.h
