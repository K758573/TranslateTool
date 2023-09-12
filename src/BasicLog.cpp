//
// Created by Shiro on 2023/8/26.
//

#include "BasicLog.h"
#include <format>
#include <iostream>
#include <Windows.h>
namespace KLogger {
std::ofstream Log::log_file_out_("log.txt", std::ios::out);

std::chrono::time_point<std::chrono::steady_clock> Log::log_start_time_ = std::chrono::steady_clock::now();

void Log::output(const std::string_view message)
{
  log_file_out_ << message << '\n' << std::flush;
//  WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), message.data(), message.size(), nullptr, nullptr);
//  WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, nullptr, nullptr);
//  FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
//  std::cout << message << '\n' << std::flush;
}
}

