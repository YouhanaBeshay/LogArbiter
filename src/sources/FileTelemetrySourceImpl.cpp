#include "FileTelemetrySourceImpl.hpp"

FileTelemetrySourceImpl::FileTelemetrySourceImpl(const std::string filePath)
    : filePath_(filePath) {}
bool FileTelemetrySourceImpl::readSource(std::string &out) {

  out.clear();

  if (file_->isOpen()) {

    out = file_->fileReadLine();
  }

  return !out.empty();
}

bool FileTelemetrySourceImpl::openSource() {

  file_ = std::make_unique<SafeFile>(filePath_);

  return file_->isOpen();
}