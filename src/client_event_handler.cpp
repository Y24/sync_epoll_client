#include "client_event_handler.h"

#include <arpa/inet.h>
EventHandler::EventHandler(int epollFd, std::string ip, int port)
    : epollFd(epollFd),
      eventManager(EventManager(epollFd)),
      sessionProductor(SessionProductor(ip, port)) {
  eventManager.add_event(STDIN_FILENO, EPOLLIN);
}
void EventHandler::do_clean() {
  // clean work
  for (auto [fd, _] : localPool) {
    close(fd);
  }
  sessionManager.clear();
  remotePool.clear();
  localPool.clear();
  flag.clear();
}
void EventHandler::do_test(std::unordered_map<int, DemoData> &data) {
  for (auto [first, second] : localPool) {
    if (flag[first] && localPool.count(first) && localPool.count(second) &&
        flag[localPool[second]]) {
      data[first] = DemoData(delivery_data, factory.toString(time(nullptr)),
                             ContentGenerator().generate(nContent));
      eventManager.modify_event(first, EPOLLOUT);
    }
  }
}
void EventHandler::do_read(int fd, std::unordered_map<int, DemoData> &data) {
  if (fd == STDIN_FILENO) {
    auto command = commandHandler.get(fd);
    std::vector<Session> sessions;
    switch (command.type) {
      case command_invalid:
        fprintf(stderr, "Input command is invalid!\n");
        break;
      case session_start:
        do_clean();
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
        break;
      case session_test:
        nContent = command.para;
        do_test(data);
        break;
      case session_stop:
        do_clean();
        break;
      default:
        break;
    }
    return;
  }
  IOHandler ioHandler(fd);
  auto res = ioHandler.read();
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
      }
      remotePool[fd] = remoteFd;
      flag[fd] = true;
      if (localPool.count(fd) == 0) {
        fprintf(stderr,
                "ClientEventHandler do_read: session_init localPool check "
                "fails\n");
      } else if (flag[localPool[fd]]) {
        sessionManager.merge({fd, localPool[fd]});
      } else {
        /// do session_pair
        data[fd] =
            DemoData(session_pair, factory.toString(remotePool[localPool[fd]]));
        eventManager.modify_event(fd, EPOLLOUT);
      }
      break;
    case session_pair:
      /// Igonor on client side by default.
      break;
    case delivery_data:
      eventManager.add_event(STDOUT_FILENO, EPOLLOUT);
      break;
    default:
      fprintf(stderr, "ClientEventHandler do_read reach default case!\n");
      break;
  }
}
void EventHandler ::do_write(int fd, std::unordered_map<int, DemoData> &data) {
  if (fd == STDOUT_FILENO) {
    time_t cur = time(nullptr);
    tm *begin = new tm;
    strftime(data[fd].getHeader().timestamp.data(), 64, "%Y-%m-%d %H:%M:%S",
             begin);
    fprintf(stdout, "Time delay: %lf ms\n",
            (double)cur - mktime(begin) / CLOCKS_PER_SEC * 1000);
    fprintf(stdout, "Data content: \n");
    delete begin;
  }
  IOHandler ioHandler(fd);
  if (!ioHandler.write(data[fd])) {
    eventManager.delete_event(fd, EPOLLOUT);
  }
  // clean work
  data.erase(fd);
}
void EventHandler::handle(epoll_event *events, int num,
                          std::unordered_map<int, DemoData> &data) {
  for (int i = 0; i < num; i++) {
    int fd = events[i].data.fd;
    if (events[i].events & EPOLLIN)
      do_read(fd, data);
    else if (events[i].events & EPOLLOUT)
      do_write(fd, data);
  }
}
