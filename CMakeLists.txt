cmake_minimum_required(VERSION 3.10)

project(hw6)
 
set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED True)

file(GLOB SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_compile_options(hw6 PRIVATE -std=c++17 -Wall -Wextra -O3)