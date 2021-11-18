cmake_minimum_required(VERSION 3.18)
project(
    Mesh
	LANGUAGES CXX C)

set(MESH_SOURCES
    Mesh.h
    Mesh.cpp
	MeshManager.h
	MeshManager.cpp
)

add_library(kalm-mesh STATIC ${MESH_SOURCES})

target_link_libraries(kalm-mesh PRIVATE assimp)
