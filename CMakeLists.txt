cmake_minimum_required(VERSION 2.23.3)

add_compile_options(-Wfatal-errors -w -g -lpthread -lm -ldl)

#find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(OpenGL)
#find_package(OpenGL REQUIRED OPTIONAL_COMPONENTS OpenGl)
#if(TARGET OpenGL::OpenGL)
#    target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL)
#else()
#    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
#endif()

project(App)

find_package(GLEW REQUIRED)

add_executable(${PROJECT_NAME}
    external/glad/glad.c
    src/main.cpp
    src/app.cpp
    src/stb_image.cpp
    src/camera.cpp
    src/model.cpp
    src/mesh.cpp
    src/shader.cpp
    src/object.cpp
    src/player.cpp
    src/timer.cpp
    src/box_collider.cpp
    src/particle_system.cpp
    src/particle.cpp
    )

add_subdirectory(external/glfw)
#add_subdirectory(external/glfw-3.3.8.bin.WIN32)
add_subdirectory(external/imgui)
add_subdirectory(external/glm)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glfw/include
    #PUBLIC external/glfw-3.3.8.bin.WIN32/include
    PUBLIC external/glad
    PUBLIC external/glm
    PUBLIC external/assimp/include
    PUBLIC external/audio/SFML-2.6.0/include
    PUBLIC /usr/include/freetype2
    PUBLIC /usr/include/freetype2/freetype
    )

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    #PRIVATE external/glfw-3.3.8.bin.WIN32/lib-mingw-w64
    PRIVATE external/imgui
    PRIVATE external/glm
    PRIVATE external/assimp/bin
    )

#include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES}
    #GLEW
    imgui
    glm::glm
    m
    OpenGL
    assimp
    freetype
    )
