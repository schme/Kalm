cmake_minimum_required(VERSION 3.18)
project(
	KalmDemo
	LANGUAGES CXX)

set (UNIX_MAIN_SOURCES
	demo_linux.cpp
)

if (UNIX)
	set( MAIN_SOURCES ${UNIX_MAIN_SOURCES})
endif()


add_executable(Demo ${MAIN_SOURCES})
target_link_libraries(Demo PRIVATE kalm-player)
target_link_libraries(Demo PRIVATE GL X11)

target_include_directories(Demo PRIVATE ..)

add_subdirectory(../player player)
