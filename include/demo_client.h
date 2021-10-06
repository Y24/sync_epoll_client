#ifndef _DEMO_CLIENT_H
#define _DEMO_CLIENT_H 1
#include <string>

#include "client_epoll_manager.h"
#include "config.h"
class DemoClient {
 private:
  std::string ip;
  int port;
  int size;
  int nEvents;

 public:
  DemoClient(std::string ip, int port);
  void work(int size = FDSIZE, int nEvents = EPOLLEVENTS);
  ~DemoClient() = default;
};
#endif  // demo_client.h
