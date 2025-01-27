cmake_minimum_required(VERSION 3.10)

project(SynthWave)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

if (NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found!")
endif()

include_directories(include ${SDL2_INCLUDE_DIRS})

set(SOURCES
    src/main.cpp
    src/render.cpp
)

add_executable(SynthWave ${SOURCES})

target_link_libraries(SynthWave ${SDL2_LIBRARIES})
