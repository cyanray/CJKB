# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.1)

project (CJJWHelper)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(libraries)

find_package(unofficial-iconv CONFIG REQUIRED)


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

target_link_libraries(${PROJECT_NAME} INTERFACE CURLWrapper)
target_link_libraries(${PROJECT_NAME} INTERFACE HTMLReader)
target_link_libraries(${PROJECT_NAME} INTERFACE HamOCR)
target_link_libraries(${PROJECT_NAME} INTERFACE unofficial::iconv::libiconv unofficial::iconv::libcharset)