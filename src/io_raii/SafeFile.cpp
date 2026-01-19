#include "SafeFile.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <vector>

SafeFile::SafeFile(const std::string filePath) {
  fd_ = open(filePath.c_str(), O_RDWR | O_CREAT | O_APPEND, S_IRWXU);

  if (fd_ == -1) {
    std::cout << "Failed to open file: " << filePath << std::endl;
  }
}


std::string SafeFile::fileReadLine() {
  if (fd_ != -1) {

    std::string buffer;
    char ch;

    while (read(fd_, &ch, 1) == 1) {
      if (ch == '\n') {
        break;
      }
      buffer += ch;
    }

    return buffer;
  }

  // return empty string (error)
  return "";
}

std::string SafeFile::fileReadLastLine() {
  if (fd_ != -1) {

    std::string buffer;
    char ch;
    // start from end of file
    off_t pos = lseek(fd_, -1, SEEK_END);

    // skip any trailing '\n'
    while (pos >= 0) {

      // error
      if (read(fd_, &ch, 1) != 1) {
        return "";
      }

      if (ch != '\n') {
        break;
      }
      // move backward
      pos--;
      lseek(fd_, pos, SEEK_SET);
    }

    // read backwards till '\n'
    while (pos >= 0) {
      // error
      if (read(fd_, &ch, 1) != 1) {
        break;
      }

      if (ch == '\n') {
        break;
      }
      buffer = ch + buffer; // pre append

      // move backward
      pos--;
      lseek(fd_, pos, SEEK_SET);
    }

    return buffer;
  }

  // return empty string (error)
  return "";
}

void SafeFile::fileWrite(const std::string &msg) {
  if (fd_ != -1) {
    write(fd_, msg.c_str(), msg.length());
  }
}

SafeFile::~SafeFile() {
  if (fd_ != -1) {
    close(fd_);
  }
}

// move semantics

SafeFile::SafeFile(SafeFile &&other) noexcept : fd_(other.fd_) {
  other.fd_ = -1;
};

SafeFile &SafeFile::operator=(SafeFile &&other) noexcept {

  // close the current file if opened
  if (fd_ != -1) {
    close(fd_);
  }
  // check for self move
  if (this != &other) {
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
};

bool SafeFile::isOpen() const { return fd_ != -1; }