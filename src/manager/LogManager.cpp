#include "LogManager.hpp"

LogManager::LogManager(size_t capacity, size_t threadPoolSize)
    : messages_(capacity),
      threadPool_(std::make_unique<ThreadPool>(threadPoolSize)) {}

void LogManager::addSink(std::unique_ptr<ILogSink> sink) {
  sinks_.push_back(std::move(sink));
}
void LogManager::addMessage(const LogMessage &msg) { messages_.tryPush(msg); }

void LogManager::writeToAll(const LogMessage &msg) {
  for (int i = 0; i < sinks_.size(); i++) {

    // var for sink to pass to the capture (instead of capturing "this" & "i")
    auto currentSink = sinks_[i];

    // use threadPool for parallelism
    threadPool_->tryEnqueueTask(
        [currentSink, msg]() { currentSink->write(msg); });
  }
}

void LogManager::flushMessages() {

  // while messages_ is not empty "tryPop doesnt return nullopt"
  while (std::optional<LogMessage> poppedMsg = messages_.tryPop()) {
    writeToAll(poppedMsg.value());
  }
}