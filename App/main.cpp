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
#include "LogManagerBuilder.hpp"
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
                     .build();  

  std::unique_ptr<ITelemetrySource> sourcefile =
      std::make_unique<FileTelemetrySourceImpl>("/proc/meminfo");

  // socket
  std::unique_ptr<ITelemetrySource> sourceSocket =
      std::make_unique<SocketTelemetrySourceImpl>(
          "/home/youhana/ITI_assignments2/Cpp_MajorTask/My_REPO/LogArbiter/"
          "provided/telemetry.sock");

  for (int i = 0; i < 10; i++) {

    if (sourcefile->openSource()) {
      std::string totalmemory, freememory, availablememory;

      if (sourcefile->readSource(totalmemory)) {
        sourcefile->readSource(freememory);
        sourcefile->readSource(availablememory);

        LogMessage message;
        message.appName = "LogArbiter";
        message.context = "Test_Phase2_File";
        message.time = "15/1/2026 12:00:0" + std::to_string(i);
        message.severity = "INFO";

        // data read from file
        message.payload =
            totalmemory + " | " + freememory + " | " + availablememory;

        manager->writeToAll(message);
      }
    }

    if (sourceSocket->openSource()) {
      std::string socketData;

      if (sourceSocket->readSource(socketData)) {
        LogMessage message;
        message.appName = "LogArbiter";
        message.context = "Test_Phase2_Socket";
        message.time = "15/1/2026 12:00:0" + std::to_string(i);
        message.severity = "INFO";

        // data read from socket
        message.payload = socketData;

        manager->writeToAll(message);
      }
    }

    // sleep for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}