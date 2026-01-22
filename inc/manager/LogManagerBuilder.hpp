#pragma once 

#include "LogManager.hpp"


// Non Enforced Builder pattern (as LogManager constructor is public [for now] )
class LogManagerBuilder {
private:
    std::vector<std::unique_ptr<ILogSink>> sinks_;
public:
    std::unique_ptr<LogManager> build();
    LogManagerBuilder& addSink(std::unique_ptr<ILogSink> sink);


};