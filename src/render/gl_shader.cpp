#include "gl_shader.h"

#include <memory>

namespace ks {
	Shader & Shader::use()
	{
		glUseProgram(program);
		return *this;
	}

    void Shader::bind(unsigned int location, float value)
	{
		glUniform1f(location, value);
	}

    void Shader::bind(unsigned int location, glm::mat4 const & matrix)
    {
		glUniformMatrix4fv(location, 1, GL_FALSE, math::value_ptr(matrix));
	}

	Shader & Shader::attach(const std::string &shaderText, Type type)
	{
		GLuint shader = create(type);
		const GLchar *shdr = shaderText.c_str();
		glShaderSource(shader, 1, &shdr, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

		if (status == false) {
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::unique_ptr<char[]> buffer(new char[length]);
			glGetShaderInfoLog(shader, length, nullptr, buffer.get());
			fprintf(stderr, "Compile error: %s", buffer.get());
		}

		glAttachShader(program, shader);
		glDeleteShader(shader);

		return *this;
	}

	GLuint Shader::create(Type type)
	{
		return glCreateShader(type);
	}

	Shader &Shader::link()
	{
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &status);
        if(status == false)
        {
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, & length);
            std::unique_ptr<char[]> buffer(new char[length]);
            glGetProgramInfoLog(program, length, nullptr, buffer.get());
            fprintf(stderr, "Link error: %s", buffer.get());
        }
        assert(status == true);
        return *this;
	}

}
