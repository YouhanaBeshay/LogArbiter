#include "LogManagerBuilder.hpp"

std::unique_ptr<LogManager> LogManagerBuilder::build() {

  auto builtLogManager = std::make_unique<LogManager>(buffer_capacity_);

  for (size_t i = 0; i < sinks_.size(); i++) {
    builtLogManager->addSink(std::move(sinks_[i]));
  }

  for (size_t i = 0; i < messages_.size(); i++) {
    builtLogManager->addMessage(messages_[i]);
  }

  return builtLogManager;
}

LogManagerBuilder& LogManagerBuilder::addSink(std::unique_ptr<ILogSink> sink) {
  sinks_.push_back(std::move(sink));
  return *this;
}

LogManagerBuilder& LogManagerBuilder::addMessage(const LogMessage& msg) {
  messages_.push_back(msg);
  return *this;
}

LogManagerBuilder& LogManagerBuilder::setBufferCapacity(size_t capacity) {
  buffer_capacity_ = capacity;
  return *this;
}