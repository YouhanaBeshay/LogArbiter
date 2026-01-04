#include "ILogSink.hpp"
#include "LogManager.hpp"
#include "LogMessage.hpp"
#include "ConsoleSinkImpl.hpp"
#include "FileSinkImpl.hpp"



int main(int argc, char const *argv[])
{
    std::unique_ptr<ILogSink> consoleSink = std::make_unique<ConsoleSinkImpl>();
    std::unique_ptr<ILogSink> fileSink = std::make_unique<FileSinkImpl>("testing.txt");

    LogManager manager;
    manager.addSink(std::move(consoleSink));
    manager.addSink(std::move(fileSink));


    LogMessage message;
    message.appName = "MyApp";
    message.context = "Test";
    message.time = "1/1/2026 12:00:00";
    message.severity = "INFO";
    message.payload = "HAPPY NEW YEAR!";
    manager.writeToAll(message);

    return 0;
}


