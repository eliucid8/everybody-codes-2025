cmake_minimum_required(VERSION 3.15)
project(AdventOfCode2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_subdirectory(src)

# find day directories (source dir scan at configure time)
file(GLOB DAY_CANDIDATES RELATIVE ${CMAKE_SOURCE_DIR}/days
     "${CMAKE_SOURCE_DIR}/days/day*")

# Build the content for the generated file:
set(GENERATED_CONTENT "")
foreach(d ${DAY_CANDIDATES})
    if (IS_DIRECTORY ${CMAKE_SOURCE_DIR}/days/${d})
        set(GENERATED_CONTENT "${GENERATED_CONTENT}add_subdirectory(days/${d})\n")
    endif()
endforeach()

# Write to build dir so source tree isn't mutated and to avoid spurious VCS diffs
set(GENERATED_FILE ${CMAKE_BINARY_DIR}/generated_days.cmake)
file(WRITE ${GENERATED_FILE} "${GENERATED_CONTENT}")

# Include the generated file
include(${GENERATED_FILE})
