#include "demo_client.h"
int main(int argc, char const *argv[]) {
  DemoClient client("127.0.0.1", 8786);
  client.work();
  return 0;
}