#include "demo_client.h"

DemoClient::DemoClient(std::string ip, int port) : ip(ip), port(port) {}
void DemoClient::work(int size, int nEvents) {
  EpollManager manager(ip, port, size, nEvents);
  manager.work();
}