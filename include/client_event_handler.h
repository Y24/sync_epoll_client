#ifndef _CLIENT_EVENT_HANDLER_H
#define _CLIENT_EVENT_HANDLER_H 1
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <string>
#include <unordered_map>

#include "command.h"
#include "command_handler.h"
#include "content_generator.h"
#include "event_manager.h"
#include "io_handler.h"
#include "session_manager.h"
#include "session_productor.h"
class EventHandler {
 private:
  int epollFd;
  int nContent;
  EventManager eventManager;
  SessionManager sessionManager;
  SessionProductor sessionProductor;
  DataFactory factory;
  CommandHandler commandHandler;
  /// clientfd: serverfd
  std::unordered_map<int, int> remotePool;
  /// clientfd: clientfd in the same session
  std::unordered_map<int, int> localPool;
  /// whether paired
  std::unordered_map<int, bool> flag;
  void do_read(int fd, std::unordered_map<int, DemoData> &data);
  void do_write(int fd, std::unordered_map<int, DemoData> &data);
  void do_clean();
  void do_test(std::unordered_map<int, DemoData> &data);

 public:
  EventHandler(int epollFd, std::string ip, int port);
  void handle(epoll_event *events, int num,
              std::unordered_map<int, DemoData> &data);
  ~EventHandler() = default;
};

#endif  // client_event_handler.h