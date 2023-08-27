#include "BasicLog.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Option.hpp>


int main()
{
//  std::async()
  curlpp::Easy easy;
  Log("bad string {0}{0}","hello");
  Log("bad string,{}{}",444,556);
  
  try {
    int a = 0;
    int b = 100;
    
  } catch (std::exception& e) {
    Log("{}", e.what());
  }
  
}