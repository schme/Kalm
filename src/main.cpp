#include "main.h"

#include "Camera.h"
#include "Input.h"
#include "gui/gui.h"
#include "Scene.h"
#include "Timeline.h"
#include "include/common.h"
#include "include/maths.h"
#include "ModelManager.h"
#include "TextureLoader.h"

#include "render/gl_model.h"
#include "render/Rendering.h"

#include <stdlib.h>
#include <stdio.h>

namespace ks {

static void resizeCallback(GLFWwindow* window, int width, int height)
{
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    ks_unused(window);
    render::setViewport(0, 0, width, height);
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

void setupModels(Scene &scene)
{
	for(size_t i=0; i < scene.models.size(); ++i) {
		Model *model = scene.models[i];
		ModelRenderAttributes &attribute = scene.attributes[i];
        render::setupModel(*model, attribute);
	}
}

void renderModels(Scene &scene, EditorState &state, const math::mat4 &view, const math::mat4 &perspective)
{
	for (size_t i=0; i < scene.models.size(); ++i) {

		const Model *model = scene.models[i];

		math::quat rotation = math::quat(model->rotation);
		math::mat4 m = math::mat4(1.0f);
		math::mat4 t = math::translate(m, model->position);
		math::mat4 r = math::toMat4(rotation);
		math::mat4 s = math::scale(m, model->scale);

		m = t * r * s;
		math::mat4 mvp = perspective * view * m;

		ModelRenderAttributes &attribute = scene.attributes[i];
        render::renderModel(state, attribute, mvp);
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
	glfwSwapInterval(1);

	EditorState state;

    render::setupEnvironment(glfwGetProcAddress);

	ShaderManager::get().createDefault();
	auto& mm = ModelManager::get();
	mm.init("assets/");
	mm.readFile("ico.dae");
	mm.readFile("HappyBuddha.obj");
    Model* model = mm.addPrimitive(PrimitiveType::Cube);
    model->scale = math::vec3(20.f, 20.f, 20.f);

    std::string cubeId = model->name;

    auto& tl = TextureLoader::get();
    tl.init("assets/");
    Texture *texture = tl.load("dish.png");

    u32 txtr = render::generateTexture();
    render::loadTexture(txtr, texture->width, texture->height, texture->channels, texture->data);

	Gui& gui = Gui::get();
	gui.init(window);

    Timeline& timeline = Timeline::get();
    timeline.init();

	Scene& mainScene = state.scene;
	mainScene.name = "Main";

	model = mainScene.addModel("ico");
    model->position = math::vec3(0.f, -3.f, -5.f);

	model = mainScene.addModel("HappyBuddha");
    model->position = math::vec3(0.f, 2.f, 5.f);

    mainScene.addModel(cubeId);

	setupModels(mainScene);

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

		render::setupFrame(width, height);

		if (state.renderWireframe) {
			render::setPolygonMode(render::PolygonMode::Line);
		}
		else {
			render::setPolygonMode(render::PolygonMode::Fill);
		}

		math::mat4 v, p;

        v = math::lookAt(camera.position, camera.position + camera.front, camera.up);
		p = math::perspective(math::radians(camera.lens.fov),
                camera.lens.aspect, camera.lens.near, camera.lens.far);

		renderModels(mainScene, state, v, p);

		Gui::get().run(state);
		Gui::get().render();

		glfwSwapBuffers(window);
	}

	Gui::get().terminate();
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


