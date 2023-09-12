#include <future>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include <windows.h>

#include "src/BasicLog.h"
#include "src/KHeader.h"
#include "src/InputFactory.h"
#include "src/ClipboardTool.h"

#include <imgui.h>

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
    klog("{}", content);
    easy.setOpt(new curlpp::Options::WriteStream(&ssm));
    easy.perform();
  } catch (std::exception &e) {
    klog("{}", e.what());
  }
  klog("translate thread : {}", thread_hash_val);
  return ssm.str();
}

bool isHotkeyTriggered(MSG &msg)
{
  if (msg.message != WM_HOTKEY) {
    return false;
  } else {
    auto mod_key = LOWORD(msg.lParam);
    auto virtual_key = HIWORD(msg.lParam);
    //      debug("触发了WM_HOTKEY,mod_key={},vkey={}", mod_key, virtual_key);
    if ((mod_key & MOD_ALT) && (mod_key & MOD_CONTROL) && (virtual_key == 0x51)) {
    } else {
      return false;
    }
  }
  return true;
}

int WINAPI
WinMain(_In_
        HINSTANCE hInstance, _In_opt_
        HINSTANCE hPrevInstance, _In_
        LPSTR lpCmdLine, _In_
        int nShowCmd)
{
  ClipboardTool ct;
  //注册快捷键
  if (RegisterHotKey(nullptr, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 0x51))
  {
    klog("{}", "hotkey ctrl + alt+q is registered");
  }
  //消息循环
  
  ///IMGUI
//  ImGui::Begin("translate tool",);
  
  
  MSG msg{};
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!isHotkeyTriggered(msg)) {
      continue;
    }
    auto selected = ct.getSelect();
    
    klog("提取到选中的文本:{}", selected);
  }
  return 0;
}