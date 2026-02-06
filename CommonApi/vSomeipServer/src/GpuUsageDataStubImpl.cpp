#include "GpuUsageDataStubImpl.hpp"

#include <CommonAPI/CommonAPI.hpp>

// random generator range:
constexpr float min = 0.0f;
constexpr float max = 100.0f;

GpuUsageDataStubImpl::GpuUsageDataStubImpl()
    : isRunning_(true), gen_(std::random_device{}()), dist_(min, max),
      LastGpuUsage_(0.0) {
  GpuRandomizerThread_ = std::thread([this]() {
    while (isRunning_) {
      // update the value and fire the event every second
      LastGpuUsage_ = dist_(gen_);
      // using the default fire event( no changes needed )
      fireNotifyGpuUsageDataChangeEvent(LastGpuUsage_);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  });
}

// not used by LogArbiter
void GpuUsageDataStubImpl::requestGpuUsageData(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    requestGpuUsageDataReply_t _reply) {

  _reply(LastGpuUsage_);
}

GpuUsageDataStubImpl::~GpuUsageDataStubImpl() {
  isRunning_ = false;
  if (GpuRandomizerThread_.joinable()) {
    GpuRandomizerThread_.join();
  }
}