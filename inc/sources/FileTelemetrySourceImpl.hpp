#pragma once

#include "ITelemetrySource.hpp"
#include "SafeFile.hpp"

#include <memory>

class FileTelemetrySourceImpl : public ITelemetrySource {
private:
  std::string
      filePath_; // we need to save the file path as this class opens
                 // the file in a separate function "openSource()" not the cntr
  std::unique_ptr<SafeFile> file_;

public:
  FileTelemetrySourceImpl(const std::string filePath);

  bool openSource() override;
  bool readSource(std::string &out) override;

  ~FileTelemetrySourceImpl() = default;
};