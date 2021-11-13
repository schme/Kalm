cmake_minimum_required(VERSION 3.18)
project(
	KalmGui
	LANGUAGES CXX)

set(GUI_SOURCES
	gui.cpp
	gui.h
)

add_library(kalm-gui STATIC ${GUI_SOURCES})

target_include_directories(kalm-gui PRIVATE imgui glfw glad)
target_link_libraries(kalm-gui PRIVATE imgui glfw glad)
