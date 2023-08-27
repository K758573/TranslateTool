cmake_minimum_required(VERSION 3.26)
project(TranslateTool)

set(CMAKE_CXX_STANDARD 20)

message("Cmake file changed")
find_package(asio CONFIG REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)
add_executable(TranslateTool BasicLog.cpp main.cpp)
target_link_libraries(TranslateTool PRIVATE unofficial::curlpp::curlpp asio::asio)
