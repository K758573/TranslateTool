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

/**
 * 仅保存公共对象，不要使用
 */
class LoggerOutput
{
protected:
  static std::ofstream log_file_out;
};


template<class ...T>
class Log :
    private LoggerOutput
{
public:
  explicit Log(const std::string_view fmt, T &&... args,
               std::source_location location = std::source_location::current())
  {
    std::string message = std::vformat(fmt, std::make_format_args(args...));
    std::string prefix = std::format("{}:{}",
                                     std::filesystem::path(location.file_name()).filename().string(),
                                     location.line());
    std::cout << prefix << ' ' << message << '\n' << std::flush;
    LoggerOutput::log_file_out << prefix << ' ' << message << '\n' << std::flush;
  }
};

template<class ...T> Log(std::string_view, T...) -> Log<T...>;
#endif //TRANSLATETOOL_BASICLOG_H
