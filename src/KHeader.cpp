//
// Created by Shiro on 2023/6/2.
//

#include "KHeader.h"
#include "Windows.h"


std::string Utils::formatErrorMessage(unsigned long error_code)
{
  std::string str("a");
  char* temp;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, 0, (LPSTR)&temp, 0, nullptr);
  str.assign(temp);
  return str;
}

std::wstring Utils::u8stringToWstring(std::string &str)
{
  auto size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int) str.size(), nullptr, 0);
  std::wstring wstr(size_needed, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int) str.size(), &wstr[0], (int) wstr.size());
  return wstr;
}

std::string Utils::wstringToU8string(const std::wstring &wstr)
{
  auto size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int) wstr.size(), nullptr, 0, nullptr, nullptr);
  std::string str(size_needed, 0);
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int) wstr.size(), &str[0], (int) str.size(), nullptr, nullptr);
  return str;
}

std::wstring Utils::formatErrorMessageW(unsigned long error_code)
{
  std::wstring str(L"a");
  wchar_t *temp;
  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                 nullptr,
                 error_code,
                 0,
                 (LPWSTR) &temp,
                 0,
                 nullptr);
  str.assign(temp);
  return str;
}
