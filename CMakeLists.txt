cmake_minimum_required(VERSION 3.18)

project(
	Kalm
	VERSION 0.1
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

add_compile_options(-Wall -Wextra)
add_subdirectory(src)
