//
// Created by Shiro on 2023/9/12.
//

#ifndef TRANSLATETOOL_TRANSLATETOOL_H
#define TRANSLATETOOL_TRANSLATETOOL_H

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

class TranslateTool
{
public:
  static std::string translate(const std::string &text);
};


#endif //TRANSLATETOOL_TRANSLATETOOL_H
