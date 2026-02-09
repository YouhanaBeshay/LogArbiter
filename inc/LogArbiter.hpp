#pragma once

#include "ILogSink.hpp"
#include "LogManager.hpp"

#include <json.hpp>

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// for graceful shutdown on ctrl+c
#include <atomic>
#include <csignal>

class LogArbiter {
private:
  nlohmann::json config_;
  std::unique_ptr<LogManager> manager_;
  std::vector<std::unique_ptr<ILogSink>> sinks_;
  std::vector<std::thread> sourceThreads_;
  std::thread writerThread_;

  static std::atomic<bool> s_isRunning ;

  // private helpers
  void createSinks();
  void buildManager();
  void createSources();
  std::optional<LogMessage> formatSourceData(std::string &rawData, std::string policyString);

public:
  LogArbiter(const std::string &configFilePath);

  // for safety (no copying)
  LogArbiter(const LogArbiter &) = delete;
  LogArbiter &operator=(const LogArbiter &) = delete;

  void start();

  // for graceful shutdown
  //(but note that this means we can only have 1 instance of LogArbiter)
  static void signalHandler(int);

  ~LogArbiter();
};