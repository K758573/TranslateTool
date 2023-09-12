//
// Created by Shiro on 2023/9/5.
//

#include "ClipboardTool.h"
#include "KHeader.h"
#include "BasicLog.h"
#include "InputFactory.h"
#include <Windows.h>

constexpr int CLIP_FORMAT_COUNT = 18;

UINT32 CLIP_FORMAT[CLIP_FORMAT_COUNT]{
  CF_UNICODETEXT, CF_TEXT, CF_BITMAP, CF_METAFILEPICT, CF_SYLK, CF_DIF, CF_TIFF, CF_OEMTEXT, CF_DIB, CF_PALETTE,
  CF_PENDATA, CF_RIFF, CF_WAVE, CF_ENHMETAFILE, CF_HDROP, CF_LOCALE, CF_DIBV5, CF_MAX,
};

constexpr int HOT_KEY_ID = 0x0011;

std::string ClipboardTool::read()
{
  //获取剪贴板数据
  buffer_.resize(0);
  HGLOBAL hglobal;
  OpenClipboard(GetForegroundWindow());
  format_ = GetPriorityClipboardFormat(CLIP_FORMAT, CLIP_FORMAT_COUNT);
  if (format_ > 0) {
    hglobal = GetClipboardData(format_);
    GlobalLock(hglobal);
    auto size = GlobalSize(hglobal);
    buffer_.assign((char *) hglobal, size);
    GlobalUnlock(hglobal);
    EmptyClipboard();
  }
  CloseClipboard();
  
  //转换数据
  if (format_ == CF_UNICODETEXT) {
    auto temp = std::wstring(reinterpret_cast<wchar_t *>(buffer_.data()));
    auto temp2 = Utils::wstringToU8string(temp);
    klog("读取选中文本:{}", temp2);
    return temp2;
  }
  return buffer_;
}

void ClipboardTool::restore()
{
  if (!data_)
    return;
  
  OpenClipboard(GetForegroundWindow());
  EmptyClipboard();
  GlobalLock(data_);
  SetClipboardData(format_, data_);
  GlobalUnlock(data_);
  CloseClipboard();
  data_ = nullptr;
}

void ClipboardTool::backup()
{
  //标准格式中的第一种符合格式，优先获取CF_TEXT
  ///获取剪贴板数据指针
  ///读取数据
  //需要备份
  if (buffer_.empty()) {
    return;
  }
  //申请内存
  auto data_size = buffer_.size();
  data_ = GlobalAlloc(0, data_size);
  GlobalLock(data_);
  memcpy(data_, buffer_.data(), data_size);
  GlobalUnlock(data_);
}

std::string ClipboardTool::getSelect()
{
  //读取旧数据，旧数据的格式
  std::string old_data = read();
  int old_format = format_;
  //申请旧数据相同大小的内存
  backup();
  if (!waitCopy()) {
    //复制失败，恢复剪贴板
    format_ = old_format;
    restore();
    return {};
  }
  //读取新复制的数据
  std::string select = read();
  //检查是不是文本
  if (format_ != CF_UNICODETEXT && format_ != CF_TEXT) {
    //不是文本，不予处理
    format_ = old_format;
    restore();
    return {};
  }
  return select;
}

bool ClipboardTool::waitCopy()
{
  klog("模拟 Ctrl C");
  auto key_down = [](auto &&key) {
    return (GetAsyncKeyState(key) & 0x8000) > 0;
  };
  //等待松开按住的快捷键
  while (key_down(VK_CONTROL) || key_down(VK_MENU) || key_down('Q')) {
    Sleep(10);
  }
  DWORD last_sequence = GetClipboardSequenceNumber();
  //输入ctrl c
  auto &inputs = InputFactory::KEYS_CONTROL_C;
  SendInput(inputs.size(), inputs.data(), sizeof(inputs[0]));
  //等待剪贴板有数据写入
  auto time = 0;
  constexpr int TIME_OUT_COUNT = 1000;
  while (time < TIME_OUT_COUNT) {
    if (last_sequence == GetClipboardSequenceNumber()) {
      time += 10;
      Sleep(10);
    } else {
      break;
    }
  }
  if (time >= TIME_OUT_COUNT) {
    return false;
  }
  return true;
}

ClipboardTool::ClipboardTool()
{
  id_ = HOT_KEY_ID;
  bool ret = RegisterHotKey(nullptr, id_, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x51);
  if (ret) {
    klog("register global key ctrl+alt+q success");
  } else {
    klog("register global key ctrl+alt+q failed");
  }
}

ClipboardTool::~ClipboardTool()
{
  bool ret = UnregisterHotKey(nullptr, id_);
  if (ret) {
    klog("unregister global key ctrl+alt+q success");
  } else {
    klog("unregister global key ctrl+alt+q failed");
  }
}

bool ClipboardTool::isHotkeyTriggered(MSG *msg)
{
  if (msg->message != WM_HOTKEY) {
    return false;
  } else {
    auto mod_key = LOWORD(msg->lParam);
    auto virtual_key = HIWORD(msg->lParam);
    klog("触发了WM_HOTKEY,mod_key={},vkey={}", mod_key, virtual_key);
    if ((mod_key & MOD_ALT) && (mod_key & MOD_CONTROL) && (virtual_key == 0x51)) {
    } else {
      return false;
    }
  }
  return true;
}
