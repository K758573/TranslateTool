//
// Created by Shiro on 2023/9/4.
//

#ifndef TRANSLATETOOL_INPUTFACTORY_H
#define TRANSLATETOOL_INPUTFACTORY_H

#include <Windows.h>
#include <vector>

class InputFactory
{

public:
  enum KEY_TYPE
  {
    KEY_DOWN, KEY_UP
  };
  
  InputFactory() = delete;
  
  static INPUT create(UINT16 vk, KEY_TYPE type);
};

#endif //TRANSLATETOOL_INPUTFACTORY_H
