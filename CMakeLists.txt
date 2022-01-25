cmake_minimum_required(VERSION 3.18)

project(
	Kalm
	VERSION 0.1
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

add_compile_options(-Wall -Wextra)

# https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
	message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

add_subdirectory(src)
