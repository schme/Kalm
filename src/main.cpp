
#include "Camera.h"
#include "Timeline.h"
#include "glad/gl.h"
#include "gui/gui.h"
#include "include/common.h"
#include "include/maths.h"
#include "mesh/MeshManager.h"
#include "render/gl_shader.h"
#include "render/gl_utils.h"

#include "imgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace ks {

	static const char* vertex_shader_text =
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
		"    gl_Position = MVP * vec4(vPos, 1.0);\n"
        "    color = vec4(vCol.r * vNorm.x, vCol.g * vNorm.y, vCol.b, vCol.a);\n"
		"    tex = uv;\n"
		"}\n";

	static const char* fragment_shader_text =
		"#version 460\n"
        "uniform float time;\n"
		"in vec4 color;\n"
        "in vec2 tex;\n"
        "out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4( 0.5f + color.r + sin(time) / 2.0f, tex.x, tex.y, 1.0);\n"
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


struct MeshRenderAttributes {
	std::string name;
	GLuint vbo, vao, ebo, shader;
	size_t indexCount;
};

struct ModelRenderAttributes {
	std::string name;
	std::vector<MeshRenderAttributes> attr;
	Shader shader;
};

static void setupModel(const Model &model, ModelRenderAttributes &mra)
{
	Shader &shader = mra.shader;
	shader
		.attach(vertex_shader_text, Shader::Type::Vert)
		.attach(fragment_shader_text, Shader::Type::Frag)
		.link();

	GLuint vbo, vao, ebo;
	GLint vpos_location, vcol_location, vnorm_location, uv_location;

	mra.name = model.name;

	glGenVertexArrays(1, &vao);

	for (const Mesh &mesh : model.meshes) {
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		// 1) bind vao, 2) bind and set vertex buffers 3) configure vertex attributes
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices.data()), mesh.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices.data()), mesh.indices.data(), GL_STATIC_DRAW);

		vpos_location = glGetAttribLocation(shader.get(), "vPos");
		vcol_location = glGetAttribLocation(shader.get(), "vCol");
		vnorm_location = glGetAttribLocation(shader.get(), "vNorm");
		uv_location = glGetAttribLocation(shader.get(), "uv");

		const MeshDescriptor &md = mesh.descriptor;
		for (const BufferDescriptor &bd : md.buffers) {
			switch(bd.type) {
				case BufferType::Vertex: {
											 glEnableVertexAttribArray(vpos_location);
											 glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
													 md.stride, (void*)bd.offset);
											 break;
										 }
				case BufferType::Color: {

											glEnableVertexAttribArray(vcol_location);
											glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
													md.stride, (void*)bd.offset);
											break;
										}
				case BufferType::Normal: {

											 glEnableVertexAttribArray(vnorm_location);
											 glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_FALSE,
													 md.stride, (void*)bd.offset);
											 break;
										 }
				case BufferType::Texcoord0: {

												glEnableVertexAttribArray(uv_location);
												glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE,
														md.stride, (void*)bd.offset);
												break;
											}
				default: {
							 fprintf(stderr, "Unhandled BufferType\n");
							 break;
						 }
			}
		}
		mra.attr.emplace_back(MeshRenderAttributes{mesh.name, vao, vbo, ebo, shader.get(), mesh.indices.size()});
	}
}

void renderModel(ModelRenderAttributes &mra, math::mat4 &mvp, float time)
{
	for (const auto &attr: mra.attr) {

		glBindVertexArray(attr.vao);

		//glBindBuffer(GL_ARRAY_BUFFER, attr.vbo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attr.ebo);

		mra.shader.use();
		mra.shader.bind("MVP", mvp);
		mra.shader.bind("time", time);

		glDrawElements(GL_TRIANGLES, attr.indexCount, GL_UNSIGNED_INT, 0);
	}
}

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

	MeshManager &mm = MeshManager::get();
	mm.init("assets/");
	mm.readFile("ico.dae");

	Gui &gui = Gui::get();
	gui.init(window);

    Timeline &timeline = Timeline::get();
    timeline.init();

	ModelRenderAttributes attributes;

	Model *model = mm.find("ico.dae");
	assert(model);
	setupModel(*model, attributes);

    Camera camera;

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

		math::mat4 m, v, p, mvp;

		m = math::mat4(1.0f);
        v = math::lookAt(camera.position, camera.target, camera.up);
		p = math::perspective(math::radians(camera.lens.fov),
                camera.lens.aspect, camera.lens.near, camera.lens.far);

		//p = math::ortho(-ratio, ratio, 1.f, -1.f);
		mvp = m * v * p;

		renderModel(attributes, mvp, time);

		Gui::get().run();

        ImGui::Begin("Camera");
        ImGui::InputFloat3("position", math::value_ptr(camera.position));
        ImGui::InputFloat3("target", math::value_ptr(camera.target));
        ImGui::InputFloat3("up", math::value_ptr(camera.up));
        ImGui::InputFloat3("front", math::value_ptr(camera.front));
        ImGui::InputFloat3("right", math::value_ptr(camera.right));

        ImGui::InputFloat("fov", &camera.lens.fov, 0.f, 360.f, "%.3f");
        ImGui::InputFloat("aspect", &camera.lens.aspect, 0.f, 3.f, "%.3f");
        ImGui::InputFloat("near", &camera.lens.near, -1000.f, 1000.f, "%.3f");
        ImGui::InputFloat("far", &camera.lens.far, -1000.f, 1000.f, "%.3f");


        ImGui::End();


		Gui::get().render();

		glfwSwapBuffers(window);
	}

	Gui::get().terminate();
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


