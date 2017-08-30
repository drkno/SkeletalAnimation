cmake_minimum_required(VERSION 3.8)
project(SkeletalAnimation)

set(CMAKE_CXX_STANDARD 11)

# config OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
LIST(APPEND LIBS ${OPENGL_gl_LIBRARY})
LIST(APPEND LIBS ${OPENGL_glu_LIBRARY})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# config freeglut
# TODO: NEED modify
add_subdirectory(3rd_party/freeglut/freeglut/freeglut)
INCLUDE_DIRECTORIES(3rd_party/freeglut/freeglut/freeglut/include)
#LIST(APPEND LIBS freeglut)
LIST(APPEND LIBS glut)

# config assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(3rd_party/assimp)
INCLUDE_DIRECTORIES(3rd_party/assimp/include)
LIST(APPEND LIBS assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

# config source code
set(APP_CORE_SRC
        SkeletalAnimation/core/scenes/BoneScene.cpp
        SkeletalAnimation/core/scenes/NormScene.cpp
        SkeletalAnimation/core/OpenGLManager.cpp
        SkeletalAnimation/core/Scene.cpp)

set(APP_MODEL_SRC
        SkeletalAnimation/model/AssimpObject.cpp
        SkeletalAnimation/model/BoxingRing.cpp
        SkeletalAnimation/model/Floor.cpp
        SkeletalAnimation/model/Tree.cpp)

set(SOURCE_FILES ${APP_CORE_SRC} ${APP_MODEL_SRC} SkeletalAnimation/Main.cpp)
add_executable(SkeletalAnimation ${SOURCE_FILES})
target_link_libraries(SkeletalAnimation ${LIBS})

file(COPY SkeletalAnimation/objects DESTINATION ${CMAKE_BINARY_DIR}/)