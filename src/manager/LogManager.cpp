#include "LogManager.hpp"

LogManager::LogManager(size_t capacity) : messages_(capacity) {}


void LogManager::addSink(std::unique_ptr<ILogSink> sink) {
  sinks_.push_back(std::move(sink));
}
void LogManager::addMessage(const LogMessage &msg) { messages_.tryPush(msg); }

void LogManager::writeToAll(const LogMessage &msg) {
  for (int i = 0; i < sinks_.size(); i++) {
    sinks_[i]->write(msg);
  }
}