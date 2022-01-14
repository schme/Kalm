#pragma once

#include <glad/gl.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "ResourceId.h"
#include "include/maths.h"
#include "include/common.h"

namespace ks {

struct Shader {

	/*
	 * Extensions supported by Khronos reference compiler:
	 * .vert, .tesc, .tese, .geom, .frag, and .comp
	 */
	enum Type : unsigned {
		Vert = 0,
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

	static inline i32 getUniform(Shader *shader, const char* name){
		return glGetUniformLocation(shader->program, name);
	}

	static inline bool hasUniform(Shader *shader, const char* name) {
		return Shader::getUniform(shader, name) != -1;
	}

	Shader* create();
	Shader* remove();
	Shader* use();
	Shader* attach(const char *shader, Type type, bool &success);
	Shader* link();
	GLuint get() { return program; }

	void bind(unsigned int location, float value);
	void bind(unsigned int location, math::vec3 value);
	void bind(unsigned int location, const math::mat4 &matrix);

	template<typename T> Shader * bind(std::string const &name, T&& value)
	{
		int location = glGetUniformLocation(program, name.c_str());
		if (location != -1)
			bind(location, std::forward<T>(value));
		return this;
	}

	template<typename T> Shader * bindNoTest(std::string const &name, T&& value)
	{
		int location = glGetUniformLocation(program, name.c_str());
		bind(location, std::forward<T>(value));
		return this;
	}

	void setSourceFile(Type type, const std::string &sourceFile);
	void getSourceFile(Type type, std::string &sourceFile);

private:
	u32 create(Type type);

	u32 program = 0;
	u32 shaderIds[Type::Count] = {0};
	std::string sourceFiles[Type::Count] = {};

	i32 status = -1;
	i32 length = -1;
};

}
