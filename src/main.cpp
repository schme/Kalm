

#include "gui/gui.h"

#include "include/common.h"
#include "include/maths.h"
#include "render/gl_shader.h"
#include "render/gl_utils.h"

#include "Timeline.h"

#include <stdlib.h>
#include <stdio.h>

namespace ks {

	static const struct
	{
		float x, y, z;
		float r, g, b;
        float u, v;
	} vertices[6] =
	{
		{ -0.8f, -0.8f, 0.f, 1.f, 0.f, 0.f, 0.0f, 1.0f },
		{ -0.8f,  0.8f, 0.f, 0.f, 1.f, 0.f, 0.0f, 0.0f },
		{  0.8f, -0.8f, 0.f, 0.f, 0.f, 1.f, 1.0f, 1.0f },
		{ -0.8f,  0.8f, 0.f, 1.f, 0.f, 0.f, 0.0f, 0.0f },
		{  0.8f,  0.8f, 0.f, 0.f, 1.f, 0.f, 1.0f, 0.0f },
		{  0.8f, -0.8f, 0.f, 0.f, 0.f, 1.f, 1.0f, 1.0f }
	};

	static const char* vertex_shader_text =
		"#version 460\n"
		"uniform mat4 MVP;\n"
        "uniform float time;\n"
		"in vec3 vPos;\n"
		"in vec3 vCol;\n"
		"in vec2 uv;\n"
		"out vec3 color;\n"
        "out vec2 tex;\n"
		"void main()\n"
		"{\n"
		"    gl_Position = MVP * vec4(vPos, 1.0);\n"
        "    color = vCol;\n"
		"    tex = uv;\n"
		"}\n";

	static const char* fragment_shader_text =
		"#version 460\n"
        "uniform float time;\n"
		"in vec3 color;\n"
        "in vec2 tex;\n"
        "out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4( 0.5f + sin(time) / 2.0f, tex.x, tex.y, 1.0);\n"
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
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            Timeline::get().togglePlayback(glfwGetTime());
        }
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

	Shader shader;
	shader
		.attach(vertex_shader_text, Shader::Type::Vert)
		.attach(fragment_shader_text, Shader::Type::Frag)
		.link();

	GLuint vbo, vao;
	GLint vpos_location, vcol_location, uv_location;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vpos_location = glGetAttribLocation(shader.get(), "vPos");
	vcol_location = glGetAttribLocation(shader.get(), "vCol");
	uv_location = glGetAttribLocation(shader.get(), "uv");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(vertices[0]), (void*) 0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(vertices[0]), (void*) (sizeof(float) * 3));
	glEnableVertexAttribArray(uv_location);
	glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE,
			sizeof(vertices[0]), (void*) (sizeof(float) * 6));


	Gui &gui = Gui::get();
	gui.init(window);
    Timeline &timeline = Timeline::get();
    timeline.init();

	while (!glfwWindowShouldClose(window))
	{
		// input
		glfwPollEvents();

		// logic

        float time = timeline.timestep(glfwGetTime());

		// render

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
		m = math::rotate(m, time, math::vec3(0.f, 0.f, 1.f));
		p = math::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mvp = m * p;

		shader.use();
		shader.bind("MVP", mvp);
		shader.bind("time", time);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		Gui::get().run();
		Gui::get().render();

		glfwSwapBuffers(window);
	}

	Gui::get().terminate();
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


