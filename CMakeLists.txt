cmake_minimum_required(VERSION 3.16)
project(OpenglLearning)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES Src/*.cpp)

add_executable(OpenglLearning main.cpp ${SOURCE_FILES})

find_package(glad CONFIG REQUIRED)
target_link_libraries(OpenglLearning PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(OpenglLearning PRIVATE glfw)
