#include "SocketTelemetrySourceImpl.hpp"

SocketTelemetrySourceImpl::SocketTelemetrySourceImpl(
    const std::string &socketPath)
    : socketPath_(socketPath) {}

bool SocketTelemetrySourceImpl::openSource() {

  // only create the socket if it doesn't exist
  // didnt need to do that with file source (as it was fast enough to open again)
  if (socket_ == nullptr) {
    socket_ = std::make_unique<SafeSocket>(socketPath_);
  }

  return socket_->isOpen();
}

bool SocketTelemetrySourceImpl::readSource(std::string &out) {
  out.clear();

  if (socket_->isOpen()) {
    out = socket_->receiveSocket();
  }
  return !out.empty();
}