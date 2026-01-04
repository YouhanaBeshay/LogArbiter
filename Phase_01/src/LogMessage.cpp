#include "LogMessage.hpp"

std::ostream &operator<<(std::ostream &outputStream, const LogMessage &msg) {
  outputStream << "[" << msg.time << "] "
               << "[" << msg.severity << "] "
               << "[" << msg.appName << "] "
               << "[" << msg.context << "] " << msg.payload;
  return outputStream;
}