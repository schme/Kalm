#pragma once
#include <glad/gl.h>

#include <string>
#include <unordered_map>

#include "ResourceId.h"
#include "include/maths.h"

namespace ks {

static const char* default_vert =
#include "shader/default.vert"

static const char* default_frag = 
#include "shader/default.frag"

static const char* pass_vert =
#include "shader/pass.vert"

static const char* pass_frag =
#include "shader/pass.frag"

struct Shader {

	enum Type : unsigned {
		Vert = GL_VERTEX_SHADER,
		Geom = GL_GEOMETRY_SHADER,
		Frag = GL_FRAGMENT_SHADER,
		Comp = GL_COMPUTE_SHADER,
	};

	static inline void unuse() {
		glUseProgram(0);
	}

	Shader* create();
	Shader* remove();
	Shader* use();
	Shader* attach(const std::string &shader, Type type, bool &success);
	Shader* link();
	GLuint get() { return program; }

	void bind(unsigned int location, float value);
	void bind(unsigned int location, math::mat4 const &matrix);

	template<typename T> Shader & bind(std::string const &name, T&& value)
	{
		int location = glGetUniformLocation(program, name.c_str());
		if (location == -1)
			fprintf(stderr, "Missing Uniform: %s\n", name.c_str());
		else
			bind(location, std::forward<T>(value));
		return *this;
	}

private:
	GLuint create(Type type);

	GLuint program = 0;
	GLint status = -1;
	GLint length = -1;
};

}
