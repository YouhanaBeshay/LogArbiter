#pragma once 

#include "LogManager.hpp"


// Non Enforced Builder pattern (as LogManager constructor is public [for now] )
class LogManagerBuilder {
private:
    std::vector<std::unique_ptr<ILogSink>> sinks_;
    std::vector<LogMessage> messages_;
    size_t buffer_capacity_ = 100;
    size_t threadPoolSize_ = 2;
public:
    std::unique_ptr<LogManager> build();
    LogManagerBuilder& addMessage(const LogMessage& msg);
    LogManagerBuilder& addSink(std::unique_ptr<ILogSink> sink);
    LogManagerBuilder& setBufferCapacity(size_t capacity);
    LogManagerBuilder& setThreadPoolSize(size_t size);


};