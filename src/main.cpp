

#include "gui/gui.h"

#include "common.h"
#include "maths.h"
#include "render/gl_shader.h"
#include "render/gl_utils.h"

#include <stdlib.h>
#include <stdio.h>

namespace ks {

	static const struct
	{
		float x, y;
		float r, g, b;
	} vertices[3] =
	{
		{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
		{  0.6f, -0.4f, 0.f, 1.f, 0.f },
		{   0.f,  0.6f, 0.f, 0.f, 1.f }
	};

	static const char* vertex_shader_text =
		"#version 460\n"
		"uniform mat4 MVP;\n"
		"attribute vec3 vCol;\n"
		"attribute vec2 vPos;\n"
		"varying vec3 color;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
		"    color = vCol;\n"
		"}\n";

	static const char* fragment_shader_text =
		"#version 460\n"
		"varying vec3 color;\n"
		"void main()\n"
		"{\n"
		"    gl_FragColor = vec4(color, 1.0);\n"
		"}\n";

	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error %d: %s\n", error, description);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ks_unused(scancode);
		ks_unused(mods);

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

} // namespace ks


using namespace ks;

int main(int, char**)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;

	window = glfwCreateWindow(1440, 900, "Kalm", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glEnable( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( render::openGlMessageCallback, 0);

	GLuint vbo, vao;
	GLint vpos_location, vcol_location;

	Shader shader;
	shader
		.attach(vertex_shader_text, Shader::Type::Vert)
		.attach(fragment_shader_text, Shader::Type::Frag)
		.link();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vpos_location = glGetAttribLocation(shader.get(), "vPos");
	vcol_location = glGetAttribLocation(shader.get(), "vCol");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
			sizeof(vertices[0]), (void*) 0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(vertices[0]), (void*) (sizeof(float) * 2));

	Gui::get().init(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;

		glViewport(0, 0, width, height);

		const math::vec3 &clear_color = Gui::get().clear_color;
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		math::mat4 m, p, mvp;

		m = math::mat4(1.0f);
		m = math::rotate(m, (float)glfwGetTime(), math::vec3(0.f, 0.f, 1.f));
		p = math::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mvp = m * p;

		shader.use();
		shader.bind("MVP", mvp);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		Gui::get().run();

		glfwSwapBuffers(window);
	}

	Gui::get().terminate();
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


