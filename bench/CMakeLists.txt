cmake_minimum_required(VERSION 3.14)

project(kharoonclient-bench LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KHAROON_INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(KHAROON_LIB ${CMAKE_CURRENT_BINARY_DIR}/../libkharoonclient.so)

add_executable(kharoon-bench src/main.cpp)

target_include_directories(kharoon-bench PRIVATE ${KHAROON_INCLUDE})
target_link_libraries(kharoon-bench PRIVATE ${KHAROON_LIB})
