#pragma once

#include <string>

class SafeFile {
private:
  int fd_ =-1;

public:
  // wraps open()
  SafeFile(const std::string filePath);


  // wraps read() only
  std::string fileReadLine();
  
  // wraps read() and lseek()
  std::string fileReadLastLine();

  // wraps write()
  void fileWrite(const std::string &msg);

  
  // wraps close()
  ~SafeFile();

  // only move semantics (noexcept)
  SafeFile(SafeFile && other) noexcept;
  SafeFile &operator=(SafeFile && other) noexcept;

  SafeFile(const SafeFile &) = delete;
  SafeFile &operator=(const SafeFile &) = delete;

  // to be used for FileSourceimpl returns
  bool isOpen() const ;
};