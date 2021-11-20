
#include "Camera.h"
#include "Timeline.h"
#include "gui/gui.h"
#include "include/common.h"
#include "include/maths.h"
#include "mesh/MeshManager.h"
#include "render/gl_model.h"
#include "render/gl_shader.h"
#include "render/gl_utils.h"
#include "Input.h"

#include "imgui.h"

#include <stdlib.h>
#include <stdio.h>

namespace ks {

void handleInput(GLFWwindow *window, InputState &input, InputState &last, Camera &camera, float delta)
{
	bool wasRmb = getLastInputState().rmb;

	if (input.esc)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (input.space && !last.space)
		Timeline::get().togglePlayback(glfwGetTime());

	if (input.rmb && !wasRmb) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (!input.rmb && wasRmb) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (input.rmb) {
		float cameraSpeed = input.shift ? 5.f : 2.f;
		updateCameraLook(camera, -input.mouseDelta);
		updateCameraPos(camera, input.forward, input.right, cameraSpeed * delta);
	}
}

} // namespace ks


using namespace ks;

int main(int, char**)
{
	glfwSetErrorCallback(errorCallback);

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

	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback( render::openGlMessageCallback, 0);

	MeshManager &mm = MeshManager::get();
	mm.init("assets/");
	mm.readFile("ico.dae");
	mm.readFile("HappyBuddha.obj");

	Gui &gui = Gui::get();
	gui.init(window);

    Timeline &timeline = Timeline::get();
    timeline.init();

	ModelRenderAttributes icoAttributes;
	ModelRenderAttributes buddhaAttributes;

	Model *icoModel = mm.find("ico.dae");
	Model *buddhaModel = mm.find("HappyBuddha.obj");
	assert(icoModel);
	assert(buddhaModel);
	setupModel(*icoModel, icoAttributes);
	setupModel(*buddhaModel, buddhaAttributes);

    Camera camera;

	float delta = 0.0f;
	float frameStart = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		{
			// input
			glfwPollEvents();
		}

		delta = glfwGetTime() - frameStart;
		frameStart = glfwGetTime();

		// logic
		float time = timeline.timestep(frameStart);
		handleInput(window, getCurrentInputState(), getLastInputState(), camera, delta);

		swapInputStates();

		// render

		int width, height;

		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);

		const math::vec3 &clear_color = Gui::get().clear_color;
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		math::mat4 m, v, p, mvp;

		m = math::mat4(1.0f);
        v = math::lookAt(camera.position, camera.position + camera.front, camera.up);
		p = math::perspective(math::radians(camera.lens.fov),
                camera.lens.aspect, camera.lens.near, camera.lens.far);

		mvp = p * v * m;

		renderModel(icoAttributes, mvp, time);

		m = math::translate(m, math::vec3(5.f, 0.f, 5.f));
		m = math::scale(m, math::vec3(5));
		mvp = p * v * m;

		renderModel(buddhaAttributes, mvp, time);

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


