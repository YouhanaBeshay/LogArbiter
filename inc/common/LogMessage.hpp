#pragma once

#include <string>
#include <ostream>

class LogMessage {
public:
  std::string appName;
  std::string context;
  std::string time;
  std::string severity;
  std::string payload;

  // defaulted special member functions
  LogMessage() = default;
  LogMessage(const LogMessage&) = default;
  LogMessage(LogMessage&&) = default;
  LogMessage& operator=(const LogMessage&) = default;
  LogMessage& operator=(LogMessage&&) = default;
  ~LogMessage() = default;

  friend std::ostream& operator<<(std::ostream& os, const LogMessage& msg);
};




