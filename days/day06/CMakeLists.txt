add_executable(day06 main.cpp)
target_include_directories(day06 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(day06 PRIVATE utils)
