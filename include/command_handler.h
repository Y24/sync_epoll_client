#ifndef _COMMAND_HANDLER_H
#define _COMMAND_HANDLER_H 1
#include "command.h"
class CommandHandler {
 private:
 public:
  CommandHandler() = default;
  Command get(int fd);
  ~CommandHandler() = default;
};
#endif  // command_handler.h