cmake_minimum_required(VERSION 3.18)
project(
	KalmExe
	LANGUAGES CXX C)

set(MAIN_SOURCES
	main.cpp
	main.h
	Timeline.h
	Timeline.cpp
    Camera.h
    Camera.cpp
	Input.h
	ResourceStorage.h
	ResourceBank.h
	Scene.h
	Scene.cpp
	SceneBank.h
	SceneBank.cpp
    Texture.cpp
    Texture.h
    TextureBank.cpp
    TextureBank.h
	ResourceId.h
    Mesh.h
    Mesh.cpp
	MeshGenerators.h
	Model.h
	Model.cpp
	ModelBank.h
	ModelBank.cpp
	Material.h
	Material.cpp
	MaterialBank.h
	MaterialBank.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(Kalm ${MAIN_SOURCES})

target_link_libraries(Kalm PRIVATE kalm-gui kalm-render kalm-filesystem)
target_link_libraries(Kalm PRIVATE glfw glad)
target_link_libraries(Kalm PRIVATE assimp)
target_link_libraries(Kalm PRIVATE stb) # stb_image: TextureBank

configure_file(include/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)

add_subdirectory(render)
add_subdirectory(gui)
add_subdirectory(filesystem)
