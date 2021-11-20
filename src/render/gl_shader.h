#pragma once
#include <glad/gl.h>

#include <string>
#include <map>

#include "include/maths.h"

namespace ks {

static const char* default_vert =
	"#version 460\n"
	"uniform mat4 MVP;\n"
	"uniform float time;\n"
	"in vec3 vPos;\n"
	"in vec4 vCol;\n"
	"in vec3 vNorm;\n"
	"in vec2 uv;\n"
	"out vec4 color;\n"
	"out vec2 tex;\n"
	"void main()\n"
	"{\n"
	"    color = vec4(vNorm.x, vNorm.y, 0.5 + sin(time) / 2.0, vCol.a);\n"
	"    tex = uv;\n"
	"    gl_Position = MVP * vec4(vPos, 1.0);\n"
	"}\n";

static const char* default_frag =
	"#version 460\n"
	"uniform mat4 MVP;\n"
	"uniform float time;\n"
	"in vec4 color;\n"
	"in vec2 tex;\n"
	"out vec4 fragColor;\n"
	"void main()\n"
	"{\n"
	"    fragColor = color;\n"
	"}\n";


struct Shader {

	enum Type : unsigned {
		Comp = GL_COMPUTE_SHADER,
		Frag = GL_FRAGMENT_SHADER,
		Geom = GL_GEOMETRY_SHADER,
		Vert = GL_VERTEX_SHADER,
	};

	Shader() { program = glCreateProgram(); }
	~Shader() { glDeleteProgram(program); }

	static inline void unuse() {
		glUseProgram(0);
	}

	Shader & use();
	Shader & attach(const std::string &shader, Type type);
	Shader & link();
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
	// Disable Copying and Assignment
	Shader(Shader const &) = delete;
	Shader & operator=(Shader const &) = delete;

	GLuint create(Type type);

	GLuint program = 0;
	GLint status = -1;
	GLint length = -1;
};

struct ShaderManager {

	Shader *createDefault() {
		Shader &shader = *create("default");
		shader
			.attach(default_vert, Shader::Type::Vert)
			.attach(default_frag, Shader::Type::Frag)
			.link();

		return &shader;
	}

	Shader *create(const std::string &name) {
		auto it = shaders.find(name);
		if (it == shaders.end()) {
			auto val = shaders.insert(std::make_pair(name, new Shader));
			if (!val.second)
				return nullptr;
			return val.first->second;
		}
		return it->second;
	}

	Shader *find(const std::string &name) {
		auto it = shaders.find(name);
		if (it == shaders.end())
			return nullptr;
		return it->second;
	}

	static ShaderManager &get() {
		static ShaderManager m;
		return m;
	}

	std::map<std::string, Shader*> shaders;
};
}
