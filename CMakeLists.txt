cmake_minimum_required(VERSION 3.26)
project(TranslateTool)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.4.3/msvc2019_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(unofficial-curlpp CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(ui)

add_executable(TranslateTool main.cpp)

set_property(TARGET TranslateTool PROPERTY WIN32_EXECUTABLE true)

target_link_libraries(TranslateTool PRIVATE mylib myui
        Qt::Widgets
        Qt::Core
        Qt::Gui
)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE Qt::Core)