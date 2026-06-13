cmake_minimum_required(VERSION 4.3)

project(homework2 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(homework2 main.cpp)

target_include_directories(homework2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(homework2 PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(homework2 PRIVATE OpenGL::GL GLUT::GLUT GLEW::GLEW glm::glm)
