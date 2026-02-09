// Phase 1 includes
#include "ConsoleSinkImpl.hpp"
#include "FileSinkImpl.hpp"
#include "ILogSink.hpp"
#include "LogManager.hpp"
#include "LogMessage.hpp"

// Phase 2 includes
#include "FileTelemetrySourceImpl.hpp"
#include "ITelemetrySource.hpp"
#include "SocketTelemetrySourceImpl.hpp"

// Phase 3 includes
#include "LogFormatter.hpp"
#include "LogManagerBuilder.hpp"
#include "LogPolicies.hpp"
#include "LogSinkFactory.hpp"

// Phase 5 includes
#include "SomeIPTelemetrySourceImpl.hpp"

// to be able to detect duplicate file sinks
#include <set>

#include "LogArbiter.hpp"

std::atomic<bool> LogArbiter::s_isRunning = false; 

//======== private helpers =========

void LogArbiter::createSinks() {

  // 1- console sink
  bool consoleSinkEnabled = config_["sinks"]["console"].value("enabled", false);
  if (consoleSinkEnabled) {
    auto consoleSink = LogSinkFactory::createSink(LogSinkType_enum::CONSOLE);
    sinks_.push_back(std::move(consoleSink));
  }

  // 2- array of file sink
  std::set<std::string> fileSinkPaths; // to detect duplicates
  if (config_["sinks"].contains("files")) {
    for (const auto &fileConfig : config_["sinks"]["files"]) {

      // check if that file sink is enabled
      bool fileSinkEnabled = fileConfig.value("enabled", false);
      if (fileSinkEnabled) {
        // check if it has a path
        auto fileSinkPath = fileConfig.value("path", "");
        if (fileSinkPath.empty()) {
          std::cout << "File sink path is missing .. will skip this sink"
                    << std::endl;
          continue;
        }

        // check if it is not a duplicate
        if (fileSinkPaths.find(fileSinkPath) == fileSinkPaths.end()) {

          fileSinkPaths.insert(fileSinkPath);
          auto fileSink =
              LogSinkFactory::createSink(LogSinkType_enum::FILE, fileSinkPath);
          sinks_.push_back(std::move(fileSink));
        }
        else {
          std::cout << "File sink path is duplicated .. will skip this sink"
                    << std::endl;
          continue;
        }
      }
    }
  }

  // warn user if no sink is enabled
  if (sinks_.empty()) {
    std::cout << "No sink is enabled in the config file!!! - this app is now "
                 "pointless :( -"
              << std::endl;
  }
}

void LogArbiter::buildManager() {

  auto manager = LogManagerBuilder();

  for (auto &sink : sinks_) {
    manager.addSink(std::move(sink));
  }
  sinks_.clear();

  // parse the capacity and thread pool size (with default values[100,2])
  size_t bufferCapacity = config_["log_manager"].value("buffer_capacity", 100);
  size_t threadPoolSize = config_["log_manager"].value("thread_pool_size", 2);

  // build
  manager_ = manager.setBufferCapacity(bufferCapacity)
                 .setThreadPoolSize(threadPoolSize)
                 .build();
}

std::optional<LogMessage>
LogArbiter::formatSourceData(std::string &rawData, std::string policyString) {
  std::optional<LogMessage> msg;
  if (policyString == "cpu") {
    auto formatter =
        std::make_unique<LogFormatter<CpuPolicy>>("LogArbiter CPU");
    msg = formatter->formatDataToLossage(rawData);
  } else if (policyString == "ram") {
    auto formatter =
        std::make_unique<LogFormatter<RamPolicy>>("LogArbiter RAM");
    msg = formatter->formatDataToLossage(rawData);
  } else if (policyString == "gpu") {
    auto formatter =
        std::make_unique<LogFormatter<GpuPolicy>>("LogArbiter GPU");
    msg = formatter->formatDataToLossage(rawData);
  } else {
    std::cout << "Unknown policy in config file: '" << policyString << "'\n";
  }
  return msg;
}

void LogArbiter::createSources() {

  // 1- thread for file source (if enabled)
  bool fileSourceEnabled = config_["sources"]["file"].value("enabled", false);
  if (fileSourceEnabled) {
    std::string pathFile = config_["sources"]["file"].value("path", "");
    // check if it has a path
    if (pathFile.empty()) {
      std::cout << "File source path is missing .. will skip this source"
                << std::endl;

    } else {
      int parseRate = config_["sources"]["file"].value("parse_rate_ms", 1000);
      std::string policy = config_["sources"]["file"].value("policy", "cpu");

      sourceThreads_.emplace_back(std::thread([this, pathFile, parseRate,
                                               policy]() {
        auto source = std::make_unique<FileTelemetrySourceImpl>(pathFile);
        while (s_isRunning) {
          if (source->openSource()) {
            std::string rawData;
            if (source->readSource(rawData))
            // format based on policy
            {
              std::optional<LogMessage> msg = formatSourceData(rawData, policy);
              if (msg.has_value()) {
                manager_->addMessage(msg.value());
              }
            }
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(parseRate));
        }
      }));
    }
  }

  // 2- thread for socket source (if enabled)
  bool socketSourceEnabled =
      config_["sources"]["socket"].value("enabled", false);
  if (socketSourceEnabled) {
    std::string pathsSocket = config_["sources"]["socket"].value("path", "");
    // check if it has a path
    if (pathsSocket.empty()) {
      std::cout << "Socket source path is missing .. will skip this source"
                << std::endl;
    } else {
      int parseRate = config_["sources"]["socket"].value("parse_rate_ms", 1000);
      std::string policy = config_["sources"]["socket"].value("policy", "ram");

      sourceThreads_.emplace_back(std::thread([this, pathsSocket, parseRate,
                                               policy]() {
        auto source = std::make_unique<SocketTelemetrySourceImpl>(pathsSocket);
         std::cout << "[Socket] Thread started with path: " << pathsSocket << "\n";
        while (s_isRunning) {
          if (source->openSource()) {
            std::string rawData;
            if (source->readSource(rawData))
            // format based on policy
            {
              std::optional<LogMessage> msg = formatSourceData(rawData, policy);
              if (msg.has_value()) {
                manager_->addMessage(msg.value());
              }
            }
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(parseRate));
        }
      }));
    }
  }

  // 3- thread for someip source (if enabled)
  bool someipSourceEnabled =
      config_["sources"]["someip"].value("enabled", false);
  if (someipSourceEnabled) {
    int parseRate = config_["sources"]["someip"].value("parse_rate_ms", 1000);
    std::string policy = config_["sources"]["someip"].value("policy", "gpu");

    sourceThreads_.emplace_back(std::thread([this, parseRate, policy]() {
      auto &source = SomeIPTelemetrySourceImpl::getInstance();
      if (!source.openSource()) {
        std::cout << "Failed to open someip source .. will skip this source\n";
        return;
      }
      while (s_isRunning) {
        std::string rawData;
        if (source.readSource(rawData))
        // format based on policy
        {
          std::optional<LogMessage> msg = formatSourceData(rawData, policy);
          if (msg.has_value()) {
            manager_->addMessage(msg.value());
          }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(parseRate));
      }
    }));
  }
}

void LogArbiter::signalHandler(int signal) { s_isRunning = false; }

//======== public methods =========

LogArbiter::LogArbiter(const std::string &configFilePath) {

  // load the config file
  std::ifstream configFile(configFilePath);
  if (!configFile.is_open()) {
    throw std::runtime_error(
        "Failed to open the config file (Please check path): " +
        configFilePath);
  }

  configFile >> config_;

  // intialize manager
  createSinks();
  buildManager();

  // gracefull shutdown on ctrl+c
  std::signal(SIGINT, LogArbiter::signalHandler);
  std::signal(SIGTERM, LogArbiter::signalHandler);
}

void LogArbiter::start() {
  s_isRunning = true;

  // create writer thread
  int sinkFlushRate = config_["log_manager"].value("sink_flush_rate_ms", 1000);

  writerThread_ = std::thread([this, sinkFlushRate]() {
    while (s_isRunning) {
      std::this_thread::sleep_for(std::chrono::milliseconds(sinkFlushRate));
      manager_->flushMessages();
    }
    // flush any remaining messages (just in case)
    manager_->flushMessages();
  });

  // create source threads
  createSources();

  if (sourceThreads_.empty()) {
    std::cout << "No sources enabled in config file!!! - this app is now "
                 "pointless :(\n";
  }

  // join all threads
  for (auto &thread : sourceThreads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  if (writerThread_.joinable()) {
    writerThread_.join();
  }
}

LogArbiter::~LogArbiter() { s_isRunning = false; }