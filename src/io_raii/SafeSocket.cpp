#include "SafeSocket.hpp"

#include <iostream>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// i guess 32 will be enough for now
static constexpr int BUF_SIZE = 32;

SafeSocket::SafeSocket(const std::string &socketPath)
    : socketPath_(socketPath) {

  // UDS socket == AF_UNIX
  // SOCK_NONBLOCK == non blocking server (also propagates to client)
  fd_ = socket(AF_UNIX, SOCK_NONBLOCK | SOCK_STREAM, 0);

  if (fd_ == -1) {
    std::cout << "Failed to open socket: " << socketPath << std::endl;
  } else {

    unlink(socketPath_.c_str()); // remove the socket if it already exists

    // bind the socket
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socketPath_.c_str());

    bind(fd_, (struct sockaddr *)&addr, sizeof(addr));
    if (fd_ == -1) {
      std::cout << "Failed to bind socket: " << socketPath << std::endl;
    } else {
      // allow only 1 connection for now
      listen(fd_, 1);
    }
  }
}

std::string SafeSocket::receiveSocket() {

  if (fd_ == -1)
    return "";

  int client_fd = accept(fd_, NULL, NULL);
  if (client_fd == -1)
    return "";

  char buffer[BUF_SIZE];

  ssize_t rec_bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

  // close the client
  close(client_fd);

  if (rec_bytes <= 0)
    return "";

  std::string result(buffer, rec_bytes);
  // remove any trailing '\n'
  while (!result.empty() && (result.back() == '\n'))
    result.pop_back();

  return result;
}

bool SafeSocket::isOpen() const { return fd_ != -1; }

SafeSocket::~SafeSocket() {
  if (fd_ != -1) {
    close(fd_);
    unlink(socketPath_.c_str());
  }
}