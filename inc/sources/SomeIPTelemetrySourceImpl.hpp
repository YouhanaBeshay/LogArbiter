#pragma once
#include "ITelemetrySource.hpp"

#include <atomic>
#include <memory>

// include the proxy
// TODO: ask if forward declaration is better
#include <v1/logarbiter/gpu/GpuUsageDataProxy.hpp>

// Both an "Adapter" and a "Singleton" Dp :(
class SomeIPTelemetrySourceImpl : public ITelemetrySource {
private:

  std::shared_ptr<v1::logarbiter::gpu::GpuUsageDataProxy<>> proxy_;

  // atomic (they will be updated in runtime thread in the callbacks notification)
  // and accessed in another threads (readSource and openSource)
  std::atomic<bool> isAvailable_;
  std::atomic<float> lastGpuReading_;

  // "Singleton" (private constructor)
  SomeIPTelemetrySourceImpl();

public:
  // "Adapter"
  bool openSource() override;
  bool readSource(std::string &out) override;

  // not copyable
  SomeIPTelemetrySourceImpl(const SomeIPTelemetrySourceImpl &) = delete;
  SomeIPTelemetrySourceImpl &
  operator=(const SomeIPTelemetrySourceImpl &) = delete;

  // move semantics
  SomeIPTelemetrySourceImpl(SomeIPTelemetrySourceImpl &&) = default;
  SomeIPTelemetrySourceImpl &operator=(SomeIPTelemetrySourceImpl &&) = default;

  // "Singleton"
  static SomeIPTelemetrySourceImpl &getInstance();

  //TODO: ask if there is a specific cleanup required
  ~SomeIPTelemetrySourceImpl() = default;
};