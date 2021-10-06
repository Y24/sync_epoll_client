#ifndef _CLIENT_EPOLL_MANAGER_H
#define _CLIENT_EPOLL_MANAGER_H 1

#include <sys/epoll.h>
#include <unistd.h>

#include <unordered_map>

#include "client_event_handler.h"
class EpollManager {
 private:
  int size;
  int epollFd;
  epoll_event* events;
  int nEvents;
  // fd: DemoData
  std::unordered_map<int, DemoData> data;

  EventHandler handler;

 public:
  EpollManager(std::string ip, int port, int size, int nEvents);
  EpollManager(EpollManager& other);
  void work();
  ~EpollManager();
};
#endif  // client_epoll_manager.h