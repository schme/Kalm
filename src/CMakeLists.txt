cmake_minimum_required(VERSION 3.18)
project(
	KalmExe
	LANGUAGES CXX)

set(MAIN_SOURCES
	main.cpp
	main.h
	Timeline.h
	Timeline.cpp
    Camera.h
    Camera.cpp
	Input.h
	Scene.h
	Scene.cpp
    ResourceBank.cpp
    ResourceBank.h
    Texture.cpp
    Texture.h
    TextureLoader.cpp
    TextureLoader.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(Kalm ${MAIN_SOURCES})

target_link_libraries(Kalm PRIVATE kalm-gui kalm-mesh kalm-render)
target_link_libraries(Kalm PRIVATE glfw glad)

configure_file(include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)

add_subdirectory(render)
add_subdirectory(mesh)
add_subdirectory(gui)
