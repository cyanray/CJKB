cmake_minimum_required (VERSION 3.1)

project (HamOCR)

set(CMAKE_CXX_STANDARD 11)

 find_package(JPEG REQUIRED)


file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC	${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${JPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${JPEG_INCLUDE_DIR})


