#include <glad/gl.h>

#include <string>

#include "maths.h"

namespace ks {
	struct Shader {

		enum Type : unsigned {
			Comp = GL_COMPUTE_SHADER,
			Frag = GL_FRAGMENT_SHADER,
			Geom = GL_GEOMETRY_SHADER,
			Vert = GL_VERTEX_SHADER,
		};

		Shader() { program = glCreateProgram(); }
		~Shader() { glDeleteProgram(program); }

		static void unuse();

		Shader & use();
		Shader & attach(const std::string &shader, Type type);
		Shader & link();
		GLuint create(Type type);
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

		GLuint program;
		GLint status;
		GLint length;
	};
}
