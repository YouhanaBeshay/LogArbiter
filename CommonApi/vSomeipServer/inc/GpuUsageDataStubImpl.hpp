#pragma once
#include "GpuUsageDataStubDefault.hpp"

#include <random>
#include <thread>

#include <atomic>

class GpuUsageDataStubImpl
    : public v1::logarbiter::gpu::GpuUsageDataStubDefault {

private:
  std::thread GpuRandomizerThread_;
  bool isRunning_ = false;

  // random number generator
  std::mt19937 gen_;
  std::uniform_real_distribution<float> dist_;

  // atomic (updated in randomzier thread and can be read by the request)
  std::atomic<float> LastGpuUsage_;

public:
  GpuUsageDataStubImpl();
  ~GpuUsageDataStubImpl();

  void requestGpuUsageData(const std::shared_ptr<CommonAPI::ClientId> _client,
                           requestGpuUsageDataReply_t _reply) override;
};