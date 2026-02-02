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

#include <chrono>
#include <thread>

int main(int argc, char const *argv[]) {
  auto consoleSink = LogSinkFactory::createSink(LogSinkType_enum::CONSOLE);
  auto fileSink =
      LogSinkFactory::createSink(LogSinkType_enum::FILE, "testing.txt");

  auto manager = LogManagerBuilder()
                     .addSink(std::move(consoleSink))
                     .addSink(std::move(fileSink))
                     .setBufferCapacity(202)
                     .setThreadPoolSize(4) // thread pool
                     .build();

  std::unique_ptr<ITelemetrySource> sourcefile =
      std::make_unique<FileTelemetrySourceImpl>(
          "/home/youhana/ITI_assignments2/Cpp_MajorTask/My_REPO/LogArbiter/"
          "provided/shell_logs.log");

  std::unique_ptr<ITelemetrySource> sourceSocket =
      std::make_unique<SocketTelemetrySourceImpl>(
          "/home/youhana/ITI_assignments2/Cpp_MajorTask/My_REPO/LogArbiter/"
          "provided/telemetry.sock");

  // thread 1: file source 
  std::thread fileReader([&]() {
    for (int i = 0; i < 10; i++) {
      if (sourcefile->openSource()) {
        std::string rawData;
        if (sourcefile->readSource(rawData)) {
          // format data
          std::unique_ptr<LogFormatter<CpuPolicy>> formatter =
              std::make_unique<LogFormatter<CpuPolicy>>("LogArbiter CPU");
          auto msg = formatter->formatDataToLossage(rawData);

          if (msg) {
            manager->writeToAll(
                msg.value()); // parallel writing on the sinks(thread pool)
          }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }
  });

  // thread 2: socket source
  std::thread socketReader([&]() {
    for (int i = 0; i < 10; i++) {
      if (sourceSocket->openSource()) {
        std::string rawData;
        if (sourceSocket->readSource(rawData)) {
          // format data
          std::unique_ptr<LogFormatter<RamPolicy>> formatter =
              std::make_unique<LogFormatter<RamPolicy>>("LogArbiter RAM");
          auto msg = formatter->formatDataToLossage(rawData);

          if (msg) {
            manager->writeToAll(
                msg.value()); // parallel writing on the sinks(thread pool)
          }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }
  });

  fileReader.join();
  socketReader.join();

  return 0;
}
