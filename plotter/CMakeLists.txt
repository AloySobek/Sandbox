cmake_minimum_required(VERSION 3.20)

project(plotter VERSION 0.1)

set(CMAKE_BUILD_TYPE Debug)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_image::SDL2_image
                      SDL2_ttf::SDL2_ttf)
