#include "gl_shader.h"

#include <memory>

namespace ks {

static GLuint getShaderGLType(Shader::Type type)
{
	switch (type) {
	case Shader::Type::Vert:
		return GL_VERTEX_SHADER;
	case Shader::Type::TessCtrl:
		return GL_TESS_CONTROL_SHADER;
	case Shader::Type::TessEval:
		return GL_TESS_EVALUATION_SHADER;
	case Shader::Type::Geom:
		return GL_GEOMETRY_SHADER;
	case Shader::Type::Frag:
		return GL_FRAGMENT_SHADER;
	case Shader::Type::Comp:
		return GL_COMPUTE_SHADER;
	default:
		return 0;
	}

}

Shader * Shader::use()
{
	glUseProgram(program);
	return this;
}

void Shader::bind(u32 location, float value)
{
	glUniform1f(location, value);
}

void Shader::bind(u32 location, math::mat4 const & matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, math::value_ptr(matrix));
}

void Shader::bind(u32 location, math::vec3 vec)
{
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::setSourceFile(Type type, const std::string &sourceFile)
{
	sourceFiles[type] = sourceFile;
}

void Shader::getSourceFile(Type type, std::string &sourceFile)
{
	sourceFile = sourceFiles[type];
}

Shader* Shader::attach(const char* shaderText, Type type, bool &success)
{
	GLuint shader = create(type);
	const GLchar *shdr = shaderText;
	glShaderSource(shader, 1, &shdr, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != true) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::unique_ptr<char[]> buffer(new char[length]);
		glGetShaderInfoLog(shader, length, nullptr, buffer.get());
		fprintf(stderr, "Compile error: %s", buffer.get());
		success = false;
	}
	else {
		success = true;
	}

	glAttachShader(program, shader);
	glDeleteShader(shader);

	shaderIds[type] = shader;

	return this;
}

Shader* Shader::create()
{
	program = glCreateProgram();
	return this;
}

Shader* Shader::remove()
{
	glDeleteProgram(program);
	program = 0;
	return this;
}

GLuint Shader::create(Type type)
{
	return glCreateShader(getShaderGLType(type));
}

Shader* Shader::link()
{
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, & length);
		std::unique_ptr<char[]> buffer(new char[length]);
		glGetProgramInfoLog(program, length, nullptr, buffer.get());
		fprintf(stderr, "Link error: %s", buffer.get());
	}

	for (u32& shaderId : shaderIds) {
		if (shaderId != 0) {
			glDetachShader(program, shaderId);
			shaderId = 0;
		}
	}
	return this;
}

}
