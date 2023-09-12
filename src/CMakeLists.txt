file(GLOB file_list *.cpp)
add_library(mylib STATIC ${file_list})

target_link_libraries(mylib PUBLIC unofficial::curlpp::curlpp)