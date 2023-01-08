cmake_minimum_required(VERSION 3.14)

project(kharoonclient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBUNWIND_INCLUDE "/usr/include")
set(LIBUNWIND_LIBS "unwind")

add_subdirectory(bench)
add_library(kharoonclient SHARED
    src/kharoon.cpp
    include/kharoon/client/kharoon.h
    src/kharoon/utils.h
    src/kharoon/context.h
    src/kharoon/context.cpp
    src/kharoon/arch/x86.cpp
)

target_include_directories(kharoonclient PUBLIC include/)
target_include_directories(kharoonclient PRIVATE ${LIBUNWIND_INCLUDE})
target_link_libraries(kharoonclient ${LIBUNWIND_LIBS})
target_compile_definitions(kharoonclient PRIVATE LIBCRASHCLIENT_LIBRARY)

