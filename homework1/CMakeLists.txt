cmake_minimum_required(VERSION 4.3)

project(homework1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(homework1 main.cpp)

target_include_directories(homework1 PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(homework1 PRIVATE OpenGL::GL GLUT::GLUT)
