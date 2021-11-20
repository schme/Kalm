
set(RENDER_SOURCES
	gl_shader.h
	gl_shader.cpp
	gl_utils.h
	gl_model.h
)

add_library(kalm-render STATIC ${RENDER_SOURCES})
target_include_directories(kalm-render PRIVATE glad)
target_link_libraries(kalm-render PRIVATE glad)
