#include <future>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <windows.h>
#include <Richedit.h>

#include "BasicLog.h"
#include "KHeader.h"
#include "InputFactory.h"

constexpr size_t BUFFER_SIZE = 10240;
constexpr size_t TIME_OUT_COUNT = 1000;
char buffer[BUFFER_SIZE + 1]{0};

std::vector<INPUT> KEYS_CONTROL_C{
  InputFactory::create(VK_CONTROL, InputFactory::KEY_DOWN), InputFactory::create('C', InputFactory::KEY_DOWN),
  InputFactory::create('C', InputFactory::KEY_UP), InputFactory::create(VK_CONTROL, InputFactory::KEY_UP)
};
std::vector<INPUT> KEYS_CONTROL_V{
  //    InputFactory::create(VK_CONTROL, InputFactory::KEY_DOWN),
  InputFactory::create('V', InputFactory::KEY_DOWN), InputFactory::create('V', InputFactory::KEY_UP),
  //    InputFactory::create(VK_CONTROL, InputFactory::KEY_UP)
};

void onHotKeyTriggered();

std::pair<int, HANDLE> backupClipboardData();

bool waitCopy();

std::string readClipboard();

void restoreClipboard(int format, HANDLE data);

std::string translate(const std::string &text)
{
  size_t thread_hash_val = std::hash<std::thread::id>()(std::this_thread::get_id());
  std::stringstream ssm;
  try {
    curlpp::Easy easy;
    std::string url = "https://api.cognitive.microsofttranslator.com/translate?api-version=3.0&to=zh-Hans";
    easy.setOpt(new curlpp::Options::Url(url));
    //    easy.setOpt(new curlpp::Options::Verbose(true));
    std::list<std::string> header;
    header.emplace_back("Ocp-Apim-Subscription-Key:5bb7d2ba83354b789774bb6b24f856d5");
    header.emplace_back("Ocp-Apim-Subscription-Region:southeastasia");
    header.emplace_back("Content-Type:application/json");
    easy.setOpt(new curlpp::Options::HttpHeader(header));
    //    std::string content = std::format("{}text{}{}", R"([{")", R"(":")", text, R"("}])");
    //    std::string content = R"([{"text":"hello,world"}])";
    std::string prefix = R"([{"text":")";
    std::string suffix = R"("}])";
    std::string content = prefix + text + suffix;
    //后缀
    //    debug(content);
    easy.setOpt(new curlpp::Options::PostFields(content));
    debug("{}", content);
    easy.setOpt(new curlpp::Options::WriteStream(&ssm));
    easy.perform();
  } catch (std::exception &e) {
    debug("{}", e.what());
  }
  debug("translate thread : {}", thread_hash_val);
  return ssm.str();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
  //注册快捷键
  if (RegisterHotKey(nullptr, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x51))  //0x42 is 'b'
  {
    debug("{}", "hotkey ctrl + alt+q is registered");
  }
  //消息循环
  MSG msg{};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (msg.message != WM_HOTKEY) {
      continue;
    } else {
      auto mod_key = LOWORD(msg.lParam);
      auto virtual_key = HIWORD(msg.lParam);
//      debug("触发了WM_HOTKEY,mod_key={},vkey={}", mod_key, virtual_key);
      if ((mod_key & MOD_ALT) && (mod_key & MOD_CONTROL) && (virtual_key == 0x51)) {
      } else {
        continue;
      }
    }
    onHotKeyTriggered();
  }
  return 0;
}

void onHotKeyTriggered()
{
  debug("触发快捷键 control + alt + q");
  
  auto ret_pair = backupClipboardData();
  HANDLE clip_raw_dump = ret_pair.second;
  int clip_raw_format = ret_pair.first;
  
  if (!waitCopy()) {
    //未能等到copy数据
    return;
  }
  
  //read data ，等待处理的字符串
  auto translated_text = readClipboard();
  
  restoreClipboard(clip_raw_format, clip_raw_dump);
  
  debug("");
}

void restoreClipboard(int format, HANDLE data)
{
  if (!data) {
    return;
  }
  OpenClipboard(GetActiveWindow());
  EmptyClipboard();
  GlobalLock(data);
  SetClipboardData(format, data);
  GlobalUnlock(data);
  CloseClipboard();
  debug("恢复剪贴板内容:{}", buffer);
}

std::string readClipboard()
{
  memset(buffer, 0, BUFFER_SIZE);
  //获取剪贴板数据
  OpenClipboard(nullptr);
  HGLOBAL hglobal;
  if ((hglobal = GetClipboardData(CF_TEXT)) || (hglobal = GetClipboardData(CF_UNICODETEXT))) {
    GlobalLock(hglobal);
    memcpy(buffer, hglobal, GlobalSize(hglobal));
    //    lstrcpy(buffer, (LPCSTR) hglobal);
    GlobalUnlock(hglobal);
  }
  CloseClipboard();
  debug("读取剪贴板:{}", buffer);
  return {buffer};
}

bool waitCopy()
{
  debug("模拟 Ctrl C");
  auto key_down = [](auto &&key) {
    return (GetAsyncKeyState(key) & 0x8000) > 0;
  };
  //等待松开按住的快捷键
  while (key_down(VK_CONTROL) || key_down(VK_MENU) || key_down('Q')) {
    Sleep(10);
  }
  //备份剪切板的当前内容
  //  OpenClipboard(nullptr);
  //  UINT clip_raw_format = 0;//第一种格式
  //  HANDLE clip_raw_data = nullptr;
  
  //  while ((clip_raw_format = EnumClipboardFormats(clip_raw_format))) {
  //    //枚举下一个
  //    clip_raw_data = GetClipboardData(clip_raw_format);
  //    if (clip_raw_data != nullptr) {
  //      debug("clip data format is {}", clip_raw_format);
  ////      break;
  //    }
  //  }
  //剪贴板ID号
  DWORD last_sequence = GetClipboardSequenceNumber();
  //输入ctrl c
  SendInput(KEYS_CONTROL_C.size(), KEYS_CONTROL_C.data(), sizeof(KEYS_CONTROL_C[0]));
  //等待剪贴板有数据写入
  auto time = 0;
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

std::pair<int, HANDLE> backupClipboardData()
{
  UINT32 clip_format[18]{
    CF_UNICODETEXT, CF_TEXT, CF_BITMAP, CF_METAFILEPICT, CF_SYLK, CF_DIF, CF_TIFF, CF_OEMTEXT, CF_DIB, CF_PALETTE,
    CF_PENDATA, CF_RIFF, CF_WAVE, CF_ENHMETAFILE, CF_HDROP, CF_LOCALE, CF_DIBV5, CF_MAX,
  };
  OpenClipboard(nullptr);
  //标准格式中的第一种符合格式，优先获取CF_TEXT
  ///获取剪贴板数据指针
  int clip_raw_format = GetPriorityClipboardFormat(clip_format, 18);
  HANDLE clip_raw_data = nullptr;
  if (clip_raw_format > 0) {
    clip_raw_data = GetClipboardData(clip_raw_format);
    //输出信息
//    memset(buffer, 0, BUFFER_SIZE);
//    GetClipboardFormatName(clip_raw_format, buffer, BUFFER_SIZE);
//    debug("clip format name = {}", buffer);
  }
  HGLOBAL clip_raw_dump = nullptr;
  ///读取数据
  if (clip_raw_data) {
    //剪贴板有内容，备份
    GlobalLock(clip_raw_data);
    auto clip_raw_size = GlobalSize(clip_raw_data);
    clip_raw_dump = GlobalAlloc(0, clip_raw_size);
    GlobalLock(clip_raw_dump);
    memcpy(clip_raw_dump, clip_raw_data, clip_raw_size);
    memset(buffer, 0, BUFFER_SIZE);
    memcpy(buffer, clip_raw_dump, clip_raw_size);
    GlobalUnlock(clip_raw_dump);
    GlobalUnlock(clip_raw_data);
  }
  EmptyClipboard();
  CloseClipboard();
  debug("剪贴板原数据:{}",buffer);
  return std::make_pair(clip_raw_format, clip_raw_dump);
}
