#pragma once

#include "ITelemetrySource.hpp"
#include "SafeSocket.hpp"

#include <memory>
#include <string>

class SocketTelemetrySourceImpl : public ITelemetrySource {
private:
  std::string socketPath_;
  std::unique_ptr<SafeSocket> socket_;

public:
  SocketTelemetrySourceImpl(const std::string &socketPath);
  bool openSource() override;
  bool readSource(std::string &out) override;
};
