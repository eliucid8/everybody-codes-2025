add_executable(day02 main.cpp)
target_include_directories(day02 PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(day02 PRIVATE utils)
