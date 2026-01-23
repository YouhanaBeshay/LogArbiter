// Phase 1 includes
#include "ConsoleSinkImpl.hpp"
#include "FileSinkImpl.hpp"
#include "ILogSink.hpp"
#include "LogManager.hpp"
#include "LogMessage.hpp"

// Phase 2 includes
#include "FileTelemetrySourceImpl.hpp"
#include "ITelemetrySource.hpp"

// Phase 3 includes
#include "LogFormatter.hpp"
#include "LogManagerBuilder.hpp"
#include "LogPolicies.hpp"
#include "LogSinkFactory.hpp"

#include "SocketTelemetrySourceImpl.hpp"

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
                     .build();

  std::unique_ptr<ITelemetrySource> sourcefile =
      std::make_unique<FileTelemetrySourceImpl>(
          "/home/youhana/ITI_assignments2/Cpp_MajorTask/My_REPO/LogArbiter/"
          "provided/shell_logs.log");

  for (int i = 0; i < 10; i++) {

    if (sourcefile->openSource()) {
      std::string rawData;

      if (sourcefile->readSource(rawData)) {

        // format data
        std::unique_ptr<LogFormatter<CpuPolicy>> formatter =
            std::make_unique<LogFormatter<CpuPolicy>>("LogArbiter");
        std::optional<LogMessage> message =
            formatter->formatDataToLossage(rawData);

        if (message.has_value()) {
          manager->writeToAll(message.value());
        }
      }
    }
    // sleep for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}