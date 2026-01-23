#pragma once

#include "ILogSink.hpp"
#include "LogMessage.hpp"
#include "RingBuffer.hpp"

#include <memory>
#include <string>
#include <vector>

class LogManager {
private:
  std::vector<std::unique_ptr<ILogSink>> sinks_;
  RingBuffer<LogMessage> messages_;

public:
  LogManager(size_t capacity);
  void addSink(std::unique_ptr<ILogSink> sink);
  void addMessage(const LogMessage &msg);
  void writeToAll(const LogMessage &msg);
};
