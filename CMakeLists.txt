cmake_minimum_required(VERSION 3.20)

project(sandbox VERSION 0.0.1 LANGUAGES CXX)

project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} glfw)
