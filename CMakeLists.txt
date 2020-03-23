cmake_minimum_required(VERSION 3.15)
project(ECG)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED SDL2)

file(GLOB ECG_SRC
        "*.h"
        "*.cpp"
        )

add_executable(ECG ${ECG_SRC})

target_link_libraries(ECG PRIVATE SDL2)