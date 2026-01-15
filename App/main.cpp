// Phase 1 includes
#include "ConsoleSinkImpl.hpp"
#include "FileSinkImpl.hpp"
#include "ILogSink.hpp"
#include "LogManager.hpp"
#include "LogMessage.hpp"

// Phase 2 includes
#include "FileTelemetrySourceImpl.hpp"
#include "ITelemetrySource.hpp"

#include <chrono>
#include <thread>

int main(int argc, char const *argv[]) {

  std::unique_ptr<ILogSink> consoleSink = std::make_unique<ConsoleSinkImpl>();
  std::unique_ptr<ILogSink> fileSink =
      std::make_unique<FileSinkImpl>("testing.txt");

  LogManager manager;
  manager.addSink(std::move(consoleSink));
  manager.addSink(std::move(fileSink));

  std::unique_ptr<ITelemetrySource> sourcefile =
      std::make_unique<FileTelemetrySourceImpl>("/home/youhana/ITI_assignments2/Cpp_MajorTask/My_REPO/LogArbiter/Phase_02/Provided/shell_logs.log");

  for (int i = 0; i < 10; i++) {
    if (sourcefile->openSource()) {
      std::string data;

      if (sourcefile->readSource(data)) {
        LogMessage message;
        message.appName = "LogArbiter";
        message.context = "Test_Phase2";
        message.time = "15/1/2026 12:00:0" + std::to_string(i);
        message.severity = "INFO";

        // data read from file
        message.payload = data;

        manager.writeToAll(message);

        // sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }
  }

  return 0;
}