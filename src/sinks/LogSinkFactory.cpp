#include "LogSinkFactory.hpp"

#include "ConsoleSinkImpl.hpp"
#include "FileSinkImpl.hpp"
#include <iostream>

std::unique_ptr<ILogSink> LogSinkFactory::createSink(LogSinkType_enum sinkType,
                                                     const std::string& sinkPath) {
  switch (sinkType) {
  case LogSinkType_enum::CONSOLE:
    return std::make_unique<ConsoleSinkImpl>();
  case LogSinkType_enum::FILE:
    if (sinkPath.empty()) {
      throw std::runtime_error("Sink path is empty");
    }
    return std::make_unique<FileSinkImpl>(sinkPath);
  case LogSinkType_enum::SOCKET:
    throw std::runtime_error("Socket sink is not implemented yet");
  default:
    throw std::runtime_error("Unknown sink type");
  }
}