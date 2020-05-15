cmake_minimum_required (VERSION 3.1)

project (HTMLReader)

set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

