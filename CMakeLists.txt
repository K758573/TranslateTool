cmake_minimum_required(VERSION 3.26)
project(TranslateTool)

set(CMAKE_CXX_STANDARD 20)

message("Cmake file changed")
find_package(asio CONFIG REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)
add_executable(TranslateTool WIN32 KHeader.cpp InputFactory.cpp BasicLog.cpp main.cpp)

add_executable(UIAutomation InputFactory.cpp UiaDocumentClient.cpp)

target_link_libraries(TranslateTool PUBLIC unofficial::curlpp::curlpp asio::asio)
target_link_libraries(TranslateTool PUBLIC onecore)