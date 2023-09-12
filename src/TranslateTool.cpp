//
// Created by Shiro on 2023/9/12.
//

#include "TranslateTool.h"
#include "BasicLog.h"
#include <future>
#include <sstream>
std::string TranslateTool::translate(const std::string &text)
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
