//
// Created by Shiro on 2023/6/2.
//

#ifndef WIN32STUDY_KHEADER_H
#define WIN32STUDY_KHEADER_H

#include <string>

class Utils
{
public:
  static std::string formatErrorMessage(unsigned long error_code);
  
  static std::wstring u8stringToWstring(std::string &str);
  
  static std::string wstringToU8string(const std::wstring &wstr);
  
  static std::wstring formatErrorMessageW(unsigned long error_code);
  
};

#endif //WIN32STUDY_KHEADER_H
