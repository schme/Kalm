#include "main.h"

#include "Camera.h"
#include "Input.h"
#include "gui/gui.h"
#include "Scene.h"
#include "Timeline.h"
#include "include/common.h"
#include "include/maths.h"
#include "MeshManager.h"
#include "render/gl_model.h"
#include "render/gl_shader.h"
#include "render/gl_utils.h"

#include <stdlib.h>
#include <stdio.h>

namespace ks {

static void resizeCallback(GLFWwindow* window, int width, int height)
{
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    render::resizeViewport(width, height);
}

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
		updateCameraPos(camera, input.forward, input.right, input.up, cameraSpeed * delta);
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
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
	glDebugMessageCallback( render::openGlMessageCallback, 0);

	EditorState state;

	ShaderManager::get().createDefault();
	MeshManager &mm = MeshManager::get();
	mm.init("assets/");
	mm.readFile("ico.dae");
	mm.readFile("HappyBuddha.obj");

	Gui &gui = Gui::get();
	gui.init(window);

    Timeline &timeline = Timeline::get();
    timeline.init();

	Scene &mainScene = state.scene;
	mainScene.name = "Main";

	Model *model = mainScene.addModel("ico");
    model->position = math::vec3(0.f, -3.f, -5.f);

	model = mainScene.addModel("HappyBuddha");
    model->position = math::vec3(0.f, 2.f, 5.f);

	mainScene.setupModels();

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
		Camera &camera = state.camera;

		state.time = timeline.timestep(frameStart);
		handleInput(window, getCurrentInputState(), getLastInputState(), camera, delta);

		swapInputStates();

		// render

		int width, height;

		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);

		const math::vec3 &clear_color = Gui::get().clear_color;
		glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		if (state.renderWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		math::mat4 v, p;

        v = math::lookAt(camera.position, camera.position + camera.front, camera.up);
		p = math::perspective(math::radians(camera.lens.fov),
                camera.lens.aspect, camera.lens.near, camera.lens.far);

		mainScene.renderModels(state, v, p);

		Gui::get().run(state);

		Gui::get().render();

		glfwSwapBuffers(window);
	}

	Gui::get().terminate();
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


