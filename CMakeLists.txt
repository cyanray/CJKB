# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.1)

project (CJKB)

set(CMAKE_CXX_STANDARD 11)

find_package(sqlite3 CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)

add_subdirectory(CJJWHelper)
# 不要构建 mirai-cpp 自带的 examples
set(MIRAI_CPP_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
add_subdirectory(mirai-cpp)


# 源码以 UTF-8 编码保存，使用 MSVC 编译器需要加编译参数提供支持
if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    message(STATUS "optional: /UTF-8")   
endif(MSVC)

aux_source_directory(./src SRCS)
add_executable(main ${SRCS})
target_link_libraries(main PRIVATE CJJWHelper)
target_link_libraries(main PRIVATE mirai-cpp)
target_link_libraries(main PRIVATE sqlite3)
target_link_libraries(main PRIVATE SQLiteCpp)
target_compile_definitions(main PRIVATE MIRAICPP_STATICLIB)
include_directories(include)
