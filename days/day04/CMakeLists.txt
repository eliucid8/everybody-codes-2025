add_executable(day04 main.cpp)
target_include_directories(day04 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(day04 PRIVATE utils)
