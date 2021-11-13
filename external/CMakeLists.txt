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

FetchContent_MakeAvailable(glfw glm)

if (HAS_EDITOR)
message( STATUS "Fetching imgui...")
FetchContent_Declare(
    imgui
    GIT_REPOSITORY git@github.com:ocornut/imgui.git
    GIT_TAG 55d35d8387c15bf0cfd71861df67af8cfbda7456 #1.85
)
FetchContent_Populate(imgui)
set (IMGUI_HEADERS
	${imgui_SOURCE_DIR}/imgui.h
	${imgui_SOURCE_DIR}/imgui_draw.h
	${imgui_SOURCE_DIR}/imgui_demo.h
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
	${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
)
set (IMGUI_SOURCES
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_demo.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/backends)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}/misc/cpp)

target_include_directories(imgui PRIVATE glfw)
target_link_libraries(imgui PRIVATE glfw)
install (FILES ${IMGUI_HEADERS} DESTINATION include)

endif()

