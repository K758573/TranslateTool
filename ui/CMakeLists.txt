file(GLOB file_list *.cpp)

add_library(myui STATIC ${file_list})

target_link_libraries(myui
        PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        mylib)