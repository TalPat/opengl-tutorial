cmake_minimum_required(VERSION 3.0)
project(renderer)

project(bomberman)
set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    include_directories(~/.brew/include)
    link_directories(~/.brew/lib)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-all)
pkg_check_modules(ASSIMP REQUIRED assimp)
pkg_check_modules(GLM REQUIRED glm)
pkg_check_modules(GLEW REQUIRED glew)

include_directories(
  ./include
  ./src
)

set(SOURCE_FILES 
  ./src/Renderer.cpp
  ./src/Camera.cpp
  ./src/Shader.cpp
  ./src/Model.cpp
  ./src/Mesh.cpp
  ./src/stb_image.h
)

add_library(renderer STATIC ${SOURCE_FILES})

include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(renderer ${SFML_LIBRARIES})
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(renderer ${GLM_LIBRARIES})
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(renderer ${ASSIMP_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(renderer ${GLEW_LIBRARIES})

# CMake instructions to test using the static lib

set( TEST_EXE rendererTest )

add_executable( ${TEST_EXE} main.cpp ) 

target_link_libraries( ${TEST_EXE} renderer )