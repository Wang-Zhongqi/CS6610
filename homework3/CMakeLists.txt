cmake_minimum_required(VERSION 4.1)

project(homework3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GL REQUIRED IMPORTED_TARGET gl)
pkg_check_modules(GLU REQUIRED IMPORTED_TARGET glu)
pkg_check_modules(GLEW REQUIRED IMPORTED_TARGET glew)
pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)
pkg_check_modules(GLM REQUIRED IMPORTED_TARGET glm)

add_executable(homework3 main.cpp 
                         src/pipeline.cpp                         
                         src/resource_engine.cpp
                         src/scane.cpp)

target_include_directories(homework3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(homework3 PRIVATE PkgConfig::GL PkgConfig::GLU PkgConfig::GLEW PkgConfig::GLFW PkgConfig::GLM)
