#pragma once
#include <cstdio>
#include <glad/gl.h>

#include "include/common.h"
#include "include/config.h"

namespace ks::render {

static void GLAPIENTRY openGlMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		const void* userParam)
{
	ks_unused(length);
	ks_unused(id);
	ks_unused(userParam);

	LogLevel logLevel = Config::get().logLevel;
	if (logLevel >= LogLevel::Error) {

		if (logLevel < LogLevel::Trace && type != GL_DEBUG_TYPE_ERROR) {
			return;
		}
		else if (logLevel < LogLevel::Error) {
			return;
		}

		fprintf( stderr, "GL CALLBACK: %s, source = 0x%x, type = 0x%x, severity = 0x%x\nmessage = %s\n",
				( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
				source, type, severity, message );
	}
}

}
