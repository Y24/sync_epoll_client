#include "client_event_handler.h"

#include <arpa/inet.h>
EventHandler::EventHandler(int epollFd, std::string ip, int port)
    : epollFd(epollFd),
      eventManager(EventManager(epollFd)),
      sessionProductor(SessionProductor(ip, port)) {
  eventManager.add_event(STDIN_FILENO, EPOLLIN);
}
void EventHandler::doClean() {
  // clean work
  for (auto [fd, _] : localPool) {
    close(fd);
  }
  sessionManager.clear();
  remotePool.clear();
  localPool.clear();
  flag.clear();
}
void EventHandler::doTest(std::unordered_map<int, DemoData> &data) {
  for (auto [first, second] : localPool) {
    if (flag[first] && localPool.count(first) && localPool.count(second) &&
        flag[localPool[second]]) {
      data[first] = DemoData(delivery_data, DemoData().now(),
                             ContentGenerator().generate(nContent));
      eventManager.add_event(first, EPOLLOUT);
      eventManager.add_event(second, EPOLLIN);
    }
  }
}
void EventHandler::outputTestResult(DemoData &data) {
  fprintf(stdout, "Time delay: %d microsecond\n",
          DemoData().diff(data.getHeader().timestamp, DemoData().now()));
  fprintf(stdout, "Data content: %s\n",
          data.getBody().content.substr(0, 10).c_str());
}
void EventHandler::doRead(int fd, std::unordered_map<int, DemoData> &data) {
  if (fd == STDIN_FILENO) {
    auto command = commandHandler.get(fd);
    std::vector<Session> sessions;
    switch (command.type) {
      case command_invalid:
        fprintf(stderr, "?_? Input command is invalid!\n");
        break;
      case session_start:
        doClean();
        sessions = sessionProductor.produce(2 * command.para);
        for (int i = 0; i < 2 * command.para; i += 2) {
          int fd1 = sessions[i].getFd().begin()->first;
          int fd2 = sessions[i + 1].getFd().begin()->first;
          eventManager.add_event(fd1, EPOLLIN);
          eventManager.add_event(fd2, EPOLLIN);
          localPool[fd1] = fd2;
          localPool[fd2] = fd1;
          flag[fd1] = flag[fd2] = false;
        }
        fprintf(stdout, "#_# %d Sessions start successful!\n", command.para);
        break;
      case session_test:
        nContent = command.para;
        fprintf(stdout,
                "^_^ With content length %d, the test result follows: \n",
                command.para);
        doTest(data);
        break;
      case session_stop:
        doClean();
        fprintf(stdout, ">_< All sessions stop successful!\n");
        break;
      case command_exit:
        doClean();
        fprintf(stdout, "&_& See you!\n");
        exit(EXIT_SUCCESS);
      default:
        break;
    }
    return;
  }
  IOHandler ioHandler(fd);
  auto res = ioHandler.read();
  std::string log =
      "do_read: fd:" + std::to_string(fd) + " ,res: " + res.toStr().c_str();
  logPool.emplace_back(log);
  std::vector<int> destination;
  int remoteFd;
  switch (res.getHeader().type) {
    case data_invalid:
      eventManager.delete_event(fd, EPOLLIN);
      sessionManager.detach(fd);
      break;
    case conn_close:
      if (!sessionManager.detach(fd)) {
        fprintf(stderr, "ClientEventHandler do_read: conn_close fails\n");
        exit(EXIT_FAILURE);
      }
      break;
    case session_init:
      /// session_init at client side serves as a trigger event of
      /// session_pair
      remoteFd = factory.stringTo<int>(res.getBody().content);
      if (remotePool.count(fd) == 1) {
        fprintf(stderr,
                "ClientEventHandler do_read: session_init remotePoll check "
                "fails\n");
        exit(EXIT_FAILURE);
      }
      remotePool[fd] = remoteFd;
      flag[fd] = true;
      if (localPool.count(fd) == 0) {
        fprintf(stderr,
                "ClientEventHandler do_read: session_init localPool check "
                "fails\n");
        exit(EXIT_FAILURE);
      }
      if (remotePool.count(localPool[fd])) {
        data[fd] =
            DemoData(session_pair, factory.toString(remotePool[localPool[fd]]));
        eventManager.modify_event(fd, EPOLLOUT);
      }
      break;
    case session_pair:
      // well, session_pair singal from server side indicates the pairing
      // result.
      if (res.getBody().content == "OK") {
        sessionManager.merge({fd, localPool[fd]});
      } else {
        sessionManager.detach(fd);
      }
      eventManager.add_event(fd, EPOLLIN);
      eventManager.add_event(localPool[fd], EPOLLIN);
      break;
    case delivery_data:
      outputTestResult(res);
      data.erase(fd);
      break;
    default:
      fprintf(stderr, "ClientEventHandler do_read reach default case!\n");
      exit(EXIT_FAILURE);
      break;
  }
}
void EventHandler ::doWrite(int fd, std::unordered_map<int, DemoData> &data) {
  if (fd == STDOUT_FILENO) {
    eventManager.delete_event(fd, EPOLLOUT);
    return;
  }
  std::string log = "do_write: fd:" + std::to_string(fd) +
                    " ,res: " + data[fd].toStr().c_str();
  logPool.emplace_back(log);
  IOHandler ioHandler(fd);
  if (!ioHandler.write(data[fd])) {
    fprintf(stderr, "ClientEventHandler do_write fails!\n");
  }
  eventManager.delete_event(fd, EPOLLOUT);
  // clean work
  data.erase(fd);
}
void EventHandler::handle(epoll_event *events, int num,
                          std::unordered_map<int, DemoData> &data) {
  for (int i = 0; i < num; i++) {
    int fd = events[i].data.fd;
    if (events[i].events & EPOLLIN)
      doRead(fd, data);
    else if (events[i].events & EPOLLOUT)
      doWrite(fd, data);
  }
}
