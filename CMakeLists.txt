cmake_minimum_required(VERSION 3.10)

project(RioDeLava CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SOURCES
    main.cpp
    cenario.cpp
    logica.cpp
    camera.cpp
    iluminacao.cpp
)

add_executable(RioDeLava ${SOURCES})

target_link_libraries(RioDeLava
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
)