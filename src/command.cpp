#include "command.h"

// Command structure:
// 1: start {nSession}
// 2: test {nContent}
// 3: stop
// 4: exit
Command ::Command(std::string target) {
  StringSpliter spliter;
  DataFactory factory;
  auto res = spliter.split(target);
  if (res[0] == "start" && res.size() == 2) {
    auto [status, result] = factory.stringTo<int>(res[1]);
    if (status && result > 0) {
      type = session_start;
      para = result;
    } else {
      type = command_invalid;
    }
  } else if (res[0] == "test" && res.size() == 2) {
    auto [status, result] = factory.stringTo<int>(res[1]);
    if (status && result > 0) {
      type = session_test;
      para = result;
    } else {
      type = command_invalid;
    }
  } else if (res[0] == "stop" && res.size() == 1) {
    type = session_stop;
  } else if (res[0] == "exit" && res.size() == 1) {
    type = command_exit;
  } else {
    type = command_invalid;
  }
}