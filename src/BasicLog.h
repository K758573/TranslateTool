//
// Created by Shiro on 2023/8/26.
//

#ifndef TRANSLATETOOL_BASICLOG_H
#define TRANSLATETOOL_BASICLOG_H

#include <fstream>
#include <format>
#include <iostream>
#include <source_location>
#include <filesystem>

namespace KLogger {
class Log
{
public:
  template<class... Types>
  static void d(std::source_location location, const std::string_view fmt, Types &&... args)
  {
    auto file_path = std::filesystem::path(location.file_name());
    auto now = std::chrono::steady_clock::now() - log_start_time_;
    auto prefix = std::format("{0:16} {1}:{2}", now, file_path.filename().string(), location.line());
    auto content = std::vformat(fmt, std::make_format_args(args...));
//    auto message = prefix + ' ' + content;
    auto message = content;
    output(message);
  }

private:
  static void output(std::string_view message);
  
  static std::chrono::time_point<std::chrono::steady_clock> log_start_time_;
  static std::ofstream log_file_out_;
};
}

#define klog(...) \
  KLogger::Log::d(std::source_location::current(),__VA_ARGS__)
  
#endif //TRANSLATETOOL_BASICLOG_H
