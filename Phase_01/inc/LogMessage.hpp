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

  friend std::ostream& operator<<(std::ostream& os, const LogMessage& msg);
};




