//
// Created by Shiro on 2023/9/4.
//

#include "InputFactory.h"

INPUT InputFactory::create(UINT16 vk, KEY_TYPE type)
{
  INPUT input = {};
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = 0;
  input.ki.wScan = MapVirtualKey(vk,MAPVK_VK_TO_VSC);
  switch (type) {
    case KEY_DOWN:
      input.ki.dwFlags = 0;
      break;
    case KEY_UP:
      input.ki.dwFlags = KEYEVENTF_KEYUP;
      break;
  }
  input.ki.dwFlags |= KEYEVENTF_SCANCODE;
  input.ki.time = 0;
  input.ki.dwExtraInfo = 0;
  return input;
}
