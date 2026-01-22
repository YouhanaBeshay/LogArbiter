#include "LogManagerBuilder.hpp"

std::unique_ptr<LogManager> LogManagerBuilder::build() {

  auto builtLogManager = std::make_unique<LogManager>();

  for (size_t i = 0; i < sinks_.size(); i++) {
    builtLogManager->addSink(std::move(sinks_[i]));
  }

  return builtLogManager;
}

LogManagerBuilder& LogManagerBuilder::addSink(std::unique_ptr<ILogSink> sink) {
  sinks_.push_back(std::move(sink));
  return *this;
}