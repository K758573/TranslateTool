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
