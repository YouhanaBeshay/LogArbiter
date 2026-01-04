#include "FileSinkImpl.hpp"

// just for error messages
#include <iostream>

FileSinkImpl::FileSinkImpl(const std::string filePath) {
  file_.open(filePath, std::ios::app); // append

  if (!file_.is_open()) {
    std::cout << "Failed to open the file: " << filePath << std::endl;
  }
}

void FileSinkImpl::write(const LogMessage &msg) {

  if (file_.is_open()) {
    // we use the operator overloaded <<
    file_ << msg << std::endl;
  }
}