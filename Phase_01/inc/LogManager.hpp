#pragma once

#include "ILogSink.hpp"
#include "LogMessage.hpp"

#include <memory>
#include <string>
#include <vector>

class LogManager {
private:
  std::vector<std::unique_ptr<ILogSink>> sinks_;
  std::vector<std::unique_ptr<LogMessage>> messages_;

public:
  void addSink(std::unique_ptr<ILogSink> sink);
  void writeToAll(const LogMessage msg);
};
