cmake_minimum_required(VERSION 3.18)
project(
	KalmExe
	LANGUAGES CXX)

set(MAIN_SOURCES
	main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_executable(Kalm ${MAIN_SOURCES})

target_include_directories(Kalm PRIVATE kalm-gui render)
target_include_directories(Kalm PRIVATE glfw glad glm)

target_link_libraries(Kalm PRIVATE kalm-gui render)
target_link_libraries(Kalm PRIVATE glfw glad)


configure_file(include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)

add_subdirectory(render)
add_subdirectory(gui)
