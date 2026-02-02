#pragma once

#include "ILogSink.hpp"
#include "LogMessage.hpp"
#include "RingBuffer.hpp"
#include <ThreadPool.hpp>

#include <memory>
#include <string>
#include <vector>

class LogManager {
private:

  // shared instead of unique to be able to pass it to the thread pool 
  std::vector<std::shared_ptr<ILogSink>> sinks_;
  RingBuffer<LogMessage> messages_;

  // thread pool to make writeToAll work in parallel for each sink
  std::unique_ptr<ThreadPool> threadPool_;

public:
  LogManager(size_t capacity, size_t threadPoolSize);
  void addSink(std::unique_ptr<ILogSink> sink);
  void addMessage(const LogMessage &msg);
  void flushMessages();
  void writeToAll(const LogMessage &msg);
};
