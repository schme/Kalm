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
    GIT_TAG 5fc92d7057913ddab9d9e4565502067dff1dcb9e
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
    GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #0.9.8
)

message( STATUS "Fetching assimp...")
FetchContent_Declare(
    assimp
    GIT_REPOSITORY git@github.com:assimp/assimp.git
    GIT_TAG 05115b07b4d7ca12a52b9fd0e0534029ac8f2cb6 #5.1.0
)

FetchContent_MakeAvailable(glfw glm assimp)

message( STATUS "Fetching imgui...")
FetchContent_Declare(
    imgui
    GIT_REPOSITORY git@github.com:ocornut/imgui.git
    GIT_TAG dc8c3618e8f8e2dada23daa1aa237626af341fd8 #1.86 wip docking
)
message( STATUS "Fetching imgui extension ImGuizmo...")
FetchContent_Declare(
    imgui-imguizmo
    GIT_REPOSITORY git@github.com:CedricGuillemet/ImGuizmo.git
    GIT_TAG 82a626f9db7dbc0c8c7a3c9353e2104cb75d8749
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

install (FILES ${IMGUI_HEADERS} DESTINATION include)
