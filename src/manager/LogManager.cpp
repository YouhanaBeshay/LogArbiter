#include "LogManager.hpp"

void LogManager::addSink(std::unique_ptr<ILogSink> sink) {
  sinks_.push_back(std::move(sink));
}
void LogManager::writeToAll(const LogMessage msg) {
  for (int i = 0; i < sinks_.size(); i++) {
    sinks_[i]->write(msg);
  }
}