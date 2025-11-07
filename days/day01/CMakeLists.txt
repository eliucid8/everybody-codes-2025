add_executable(day01 main.cpp)
target_include_directories(day01 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(day01 PRIVATE utils)
