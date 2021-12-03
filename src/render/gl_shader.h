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

	Shader() { program = glCreateProgram(); }
	~Shader() { glDeleteProgram(program); }

	static inline void unuse() {
		glUseProgram(0);
	}

	Shader & use();
	Shader & attach(const std::string &shader, Type type, bool &success);
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
        bool res = false;
		shader
			.attach(default_vert, Shader::Type::Vert, res)
			.attach(default_frag, Shader::Type::Frag, res)
			.link();

		return &shader;
	}

	Shader *createPass() {
		Shader &shader = *create("pass");
		bool res = false;
		shader
			.attach(pass_vert, Shader::Type::Vert, res)
			.attach(pass_frag, Shader::Type::Frag, res)
			.link();

		return &shader;
	}

	Shader *create(const std::string &name) {
		auto it = storage.find(name);
		if (it == storage.end()) {
			auto val = storage.emplace(std::make_pair(name, new Shader));
			if (!val.second)
				return nullptr;
			return val.first->second;
		}
		return it->second;
	}

	Shader *find(const std::string &name) {
		auto it = storage.find(name);
		if (it == storage.end())
			return nullptr;
		return it->second;
	}

	static ShaderManager &get() {
		static ShaderManager m;
		return m;
	}

	std::unordered_map<ResourceId, Shader*> storage;
};
}
