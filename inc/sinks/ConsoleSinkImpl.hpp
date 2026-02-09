#pragma once

#include "ILogSink.hpp"

#include <mutex>

class ConsoleSinkImpl : public ILogSink {

private:
  // for thread safety (3 source try to write to console at the same time)
  std::mutex consoleMutex_;

public:
  void write(const LogMessage &msg) override;
};