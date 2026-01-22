#pragma once

#include "ILogSink.hpp"
#include "LogEnums.hpp"

#include <memory>


// factory "method" pattern
class LogSinkFactory {
public:
  // path is optional (used only for file sinks)
  // TODO: ask if there is a better way to handle it
  static std::unique_ptr<ILogSink> createSink(LogSinkType_enum sinkType,
                                              const std::string& sinkPath = "");
};
