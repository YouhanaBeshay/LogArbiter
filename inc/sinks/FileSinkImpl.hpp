#pragma once

#include "LogMessage.hpp"
#include "ILogSink.hpp"

// decieded to use fstream instead of normal system calls (to be able to use the operator << of LogMessage)
#include <fstream>

#include <mutex>

class FileSinkImpl : public ILogSink {
private:
    std::ofstream file_;
    // for thread safety
    std::mutex fileMutex_;
public:
    FileSinkImpl(const std::string filePath);
    void write(const LogMessage & msg) override;
    
};