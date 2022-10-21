cmake_minimum_required(VERSION 3.23)
project(sdl_tutorial)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory(cmake_utils)

init_path()
init_definition()
link_sdl()
link_sdl_image()

add_subdirectory(src)


add_executable(sdl_tutorial main.cpp)