cmake_minimum_required(VERSION 3.15)
project(ECG)

set(CMAKE_CXX_STANDARD 14)

file(GLOB ECG_SRC
        "*.h"
        "*.cpp"
        )


add_executable(ECG ${ECG_SRC})