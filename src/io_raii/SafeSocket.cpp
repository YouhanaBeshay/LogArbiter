#include "SafeSocket.hpp"

#include <iostream>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


SafeSocket::SafeSocket(const std::string &socketPath) {

  // UDS socket == AF_UNIX
  // SOCK_NONBLOCK == non blocking client
  fd_ = socket(AF_UNIX, SOCK_NONBLOCK | SOCK_STREAM, 0);

  if (fd_ == -1) {
    std::cout << "Failed to open socket: " << socketPath << std::endl;
  } else {

    // bind the socket
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socketPath.c_str());

    connect(fd_, (struct sockaddr *)&addr, sizeof(addr));
    if (fd_ == -1) {
      std::cout << "Failed to connect to server: " << socketPath << std::endl;
      close(fd_);
      fd_ = -1;
    }
  }
}

std::string SafeSocket::receiveSocket() {

  if (fd_ == -1)
    return "";
  std::string receivedline;
  char ch;

  // read only one line (until \n)
  while (read(fd_, &ch, 1) == 1) {
    if (ch == '\n') {
      break;
    } else
      receivedline += ch;
  }

  return receivedline;
}

bool SafeSocket::isOpen() const { return fd_ != -1; }

SafeSocket::~SafeSocket() {
  if (fd_ != -1) {
    close(fd_);
  }
}