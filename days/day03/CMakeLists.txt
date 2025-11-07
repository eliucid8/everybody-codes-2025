add_executable(day03 main.cpp)
target_include_directories(day03 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(day03 PRIVATE utils)
