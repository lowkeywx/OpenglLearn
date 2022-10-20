cmake_minimum_required(VERSION 3.16)
project(OpenglLearning)

set(CMAKE_CXX_STANDARD 17)
add_executable(OpenglLearning main.cpp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(OpenglLearning PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(OpenglLearning PRIVATE glfw)
