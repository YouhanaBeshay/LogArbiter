#pragma once

#include "LogEnums.hpp"
#include "LogMessage.hpp"
#include "LogPolicies.hpp"

#include <chrono>
#include <string>
#include <iomanip>

// template classes are implemented in header files :(
template <typename Policy> class LogFormatter {
private:
  std::string appName_;

  std::string msgDescription(float val) {

    std::string val_string = std::to_string(val);
    std::string ctx_string =std::string ( magic_enum::enum_name(Policy::context));
    std::string unit_string = std::string ( Policy::unit);

    std::string returned_str;

    // only if severity is critical we make a custom payload
    if (Policy::inferSeverity(val) == SeverityLvl_enum::CRITICAL) {
      returned_str = "Read from " + ctx_string +
                     " " + val_string + unit_string +
                     " (Please take action immediately!)";
    } else {
      returned_str = "Read from " + ctx_string +
                     " " + val_string + unit_string;
    }
    return returned_str;
  }
  std::string currentTimeStamp() {
    auto sys_clk = std::chrono::system_clock::now();
    auto sys_clkin_time_t = std::chrono::system_clock::to_time_t(sys_clk);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&sys_clkin_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
  }

public:
  LogFormatter(const std::string &appName) : appName_(appName){};

  std::optional<LogMessage> formatDataToLossage(const std::string &raw) {

    // we use try catch to handle 'stof' execptions

    try {
      float val = std::stof(raw);
      LogMessage msg;
      msg.appName = appName_;

      msg.context = std::string ( magic_enum::enum_name(Policy::context));
      msg.time = currentTimeStamp();
      msg.severity = std::string ( magic_enum::enum_name(Policy::inferSeverity(val)));
      msg.payload = msgDescription(val);

      return msg;

    }
    // catch any exception
    catch (...) {
      return std::nullopt;
    }
  }

  ~LogFormatter() = default;
};