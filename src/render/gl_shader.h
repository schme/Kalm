#pragma once
#include <glad/gl.h>

#include <string>
#include <unordered_map>

#include "ResourceId.h"
#include "include/maths.h"
#include "include/common.h"

namespace ks {

static const char* default_vert =
#include "shader/default.vert"

static const char* default_frag = 
#include "shader/default.frag"

static const char* pass_vert =
#include "shader/pass.vert"

static const char* pass_frag =
#include "shader/pass.frag"

static const char* matcap_vert =
#include "shader/matcap.vert"

static const char* matcap_frag =
#include "shader/matcap.frag"

struct Shader {

	enum Type : unsigned {
		Vert = 1,
		TessCtrl,
		TessEval,
		Geom,
		Frag,
		Comp,
		Count,
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
	void bind(unsigned int location, math::vec3 value);
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
	u32 create(Type type);

	u32 program = 0;
	u32 shaderIds[Type::Count] = {0};
	i32 status = -1;
	i32 length = -1;
};

}
