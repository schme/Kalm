cmake_minimum_required(VERSION 3.18)
include(FetchContent)

message( STATUS "Fetching glfw...")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY git@github.com:glfw/glfw.git
    GIT_TAG d3b73abba0cab8cbb2a638151477f54d8502a07e # 3.3.5
)

message( STATUS "Fetching glad...")
FetchContent_Declare(
    glad
    GIT_REPOSITORY git@github.com:Dav1dde/glad.git
    GIT_TAG d08b1aa01f8fe57498f04d47b5fa8c48725be877 # 2.0.4
)
FetchContent_Populate(glad)
include(${glad_SOURCE_DIR}/cmake/CMakeLists.txt)
glad_add_library(glad STATIC
    LOCATION ${glad_BINARY_DIR}
    API gl:core=4.6
    LANGUAGE cpp
)

message( STATUS "Fetching glm...")
FetchContent_Declare(
    glm
    GIT_REPOSITORY git@github.com:g-truc/glm.git
    GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e # 0.9.9.8
)

message( STATUS "Fetching assimp...")
FetchContent_Declare(
    assimp
    GIT_REPOSITORY git@github.com:assimp/assimp.git
    GIT_TAG 9519a62dd20799c5493c638d1ef5a6f484e5faf1 # 5.2.5 Bugfixes

)

message( STATUS "Fetching tinyxml2...")
FetchContent_Declare(
	tinyxml2
    GIT_REPOSITORY git@github.com:leethomason/tinyxml2.git
    GIT_TAG a9773976845b19e89020c1215781e71116477ef1 # master @ 25.1.2022
)

FetchContent_MakeAvailable(glfw glm assimp tinyxml2)

message( STATUS "Fetching stb libraries...")
FetchContent_Declare(
    stb
	GIT_REPOSITORY git@github.com:nothings/stb.git
	GIT_TAG af1a5bc352164740c1cc1354942b1c6b72eacb8a
)

FetchContent_Populate(stb)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR}/)

message( STATUS "Fetching imgui...")
FetchContent_Declare(
    imgui
    GIT_REPOSITORY git@github.com:ocornut/imgui.git
    GIT_TAG e25e4526cd41cd6536194de098540d54244f54e9 # docking @ 1.3.2023
)
message( STATUS "Fetching imgui extension ImGuizmo...")
FetchContent_Declare(
    imgui-imguizmo
    GIT_REPOSITORY git@github.com:CedricGuillemet/ImGuizmo.git
    GIT_TAG fbc3614abcc642de7f3868cc0d68a35503ca9a4e # 1.3.2023
)

FetchContent_Populate(imgui)
FetchContent_Populate(imgui-imguizmo)

message (STATUS "imgui source directory: ${imgui_SOURCE_DIR}")
message (STATUS "imgui-guizmo source directory: ${imgui-imguizmo_SOURCE_DIR}")

set (IMGUI_HEADERS
	${imgui_SOURCE_DIR}/imgui.h
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
	${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
    ${imgui-imguizmo_SOURCE_DIR}/GraphEditor.h
    ${imgui-imguizmo_SOURCE_DIR}/ImCurveEdit.h
    ${imgui-imguizmo_SOURCE_DIR}/ImGradient.h
    ${imgui-imguizmo_SOURCE_DIR}/ImGuizmo.h
    ${imgui-imguizmo_SOURCE_DIR}/ImSequencer.h
    ${imgui-imguizmo_SOURCE_DIR}/ImZoomSlider.h
)
set (IMGUI_SOURCES
    ${IMGUI_HEADERS}
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_demo.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui-imguizmo_SOURCE_DIR}/GraphEditor.cpp
    ${imgui-imguizmo_SOURCE_DIR}/ImCurveEdit.cpp
    ${imgui-imguizmo_SOURCE_DIR}/ImGradient.cpp
    ${imgui-imguizmo_SOURCE_DIR}/ImGuizmo.cpp
    ${imgui-imguizmo_SOURCE_DIR}/ImSequencer.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${imgui_SOURCE_DIR}/misc/cpp
        ${imgui-imguizmo_SOURCE_DIR}
)

target_link_libraries(imgui PRIVATE glfw)
