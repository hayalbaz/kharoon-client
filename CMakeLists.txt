cmake_minimum_required(VERSION 3.14)

project(kharoonclient LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBUNWIND_INCLUDE "/usr/include")
set(LIBUNWIND_LIBS "unwind")

add_subdirectory(bench)
add_library(kharoonclient SHARED
    #TODO: need some sort of flag to choose between different architectures and operating systems.
    src/kharoon/arch/x86.cpp

    include/kharoon/client/kharoon.h
    src/kharoon.cpp
    src/kharoon/utils.h
    src/kharoon/utils.cpp
    src/kharoon/context.h
    src/kharoon/context.cpp
    src/kharoon/os/compatibility.h
    src/kharoon/os/compatibility.cpp
    src/kharoon/os/linux_os.h
    src/kharoon/os/linux_os.cpp
)

target_include_directories(kharoonclient PUBLIC include/)
target_include_directories(kharoonclient PRIVATE ${LIBUNWIND_INCLUDE})
target_link_libraries(kharoonclient ${LIBUNWIND_LIBS})
target_compile_definitions(kharoonclient PRIVATE LIBCRASHCLIENT_LIBRARY)

