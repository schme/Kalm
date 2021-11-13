
set(RENDER_SOURCES
	gl_shader.h
	gl_shader.cpp
	gl_utils.h
)

add_library(render STATIC ${RENDER_SOURCES})
target_include_directories(render PRIVATE glad)
target_link_libraries(render PRIVATE glad)
