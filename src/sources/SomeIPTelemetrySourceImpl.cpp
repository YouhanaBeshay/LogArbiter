#include "SomeIPTelemetrySourceImpl.hpp"

#include <CommonAPI/CommonAPI.hpp>

SomeIPTelemetrySourceImpl &SomeIPTelemetrySourceImpl::getInstance() {
  static SomeIPTelemetrySourceImpl instance;
  return instance;
}

SomeIPTelemetrySourceImpl::SomeIPTelemetrySourceImpl()
    : isAvailable_(false), lastGpuReading_(0.0f) {}

bool SomeIPTelemetrySourceImpl::openSource() {
  // runtime
  auto runtime = CommonAPI::Runtime::get();
  if (!runtime) {
    return false;
  }
  // build the proxy
  std::string domain = "local";
  std::string instance = "logarbiter.gpu.GpuUsageData";
  proxy_ = runtime->buildProxy<v1::logarbiter::gpu::GpuUsageDataProxy>(
      domain, instance);
  if (!proxy_) {
    return false;
  }

  // subscribe to the event status ( to know when its available )
  proxy_->getProxyStatusEvent().subscribe(
      [this](CommonAPI::AvailabilityStatus status) {
        if (status == CommonAPI::AvailabilityStatus::AVAILABLE) {
          isAvailable_ = true;

          // if available subscribe to the event
          proxy_->getNotifyGpuUsageDataChangeEvent().subscribe(
              [this](float value) { lastGpuReading_ = value; });
        }
      });

  return true;
}

bool SomeIPTelemetrySourceImpl::readSource(std::string &out) {
  out.clear();
  if (isAvailable_) {
    out = std::to_string(lastGpuReading_);
  }
  return !out.empty();
}
