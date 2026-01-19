#pragma once

#include <string>

// This class is for UDS socket **ONLY**
class SafeSocket {
private:
  int fd_ = -1;

  // needed as socket cleanup(destructor) requires the path
  std::string socketPath_;

public:
  SafeSocket(const std::string& socketPath);

  std::string receiveSocket();

  // move semantics
  SafeSocket(SafeSocket &&other) noexcept;
  SafeSocket &operator=(SafeSocket &&other) noexcept;

  // no copy
  SafeSocket(const SafeSocket &) = delete;
  SafeSocket &operator=(const SafeSocket &) = delete;

  ~SafeSocket();

  bool isOpen() const;
};