#include "main.h"

#include "Camera.h"
#include "Input.h"
#include "gui/gui.h"
#include "Scene.h"
#include "Timeline.h"
#include "include/common.h"
#include "include/maths.h"
#include "ModelBank.h"
#include "TextureBank.h"
#include "SceneBank.h"
#include "MaterialBank.h"

#include "filesystem/FileWatcher.h"

#include "render/glRendering.h"
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
			updateFreeCameraLook(camera, -input.mouseDelta);
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
		render::setupFrameRenderModels();

		for (size_t i=0; i < scene.models.size(); ++i) {

			const Model *model = scene.models[i];
			if (model->isHidden)
				continue;

			math::quat rotation = math::quat(math::radians(model->rotation));
			math::mat4 m = math::mat4(1.0f);
			math::mat4 t = math::translate(m, model->position);
			math::mat4 r = math::toMat4(rotation);
			math::mat4 s = math::scale(m, model->scale);

			m = t * r * s;
			ModelRenderAttributes &attribute = scene.attributes[i];
			render::renderModel(state, model->material, attribute, m, view, perspective);
		}
	}

	std::vector<ResourceId> shaderRecompiles;

	static void shaderFileChanged(FileWatcher::EventParams params)
	{
		if (!(params.eventMask & FileWatcher::Event::Modified))
			return;

		std::string shaderId(params.filename);
		removeExtension(shaderId);

		shaderRecompiles.push_back(shaderId);
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

	int width = 1440;
	int height = 900;

	GLFWwindow* window = nullptr;

	window = glfwCreateWindow(width, height, "Kalm", nullptr, nullptr);
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

	state.width = width;
	state.height = height;

	render::setupEnvironment(glfwGetProcAddress);

	// Initialize modules

	FileWatcher::init();
	ModelBank::get().init();
	TextureBank::get().init();
	ShaderBank::get().init();
	Timeline::get().init();
	Gui::get().init(window);

	{
		std::string shaderFolder = getEditorState().projectRoot + getEditorState().assetPrefix;
		FileWatcher::addWatcher(shaderFolder.c_str(), shaderFileChanged);
	}

	{
		// Create defaults
		Material *matcap = MaterialBank::get().load("matcap");
		matcap->shader = ResourceId("matcap");
		matcap->texture0 = ResourceId("matcap.png");

		MaterialBank::get().loadResourcesIfNeeded(ResourceId("matcap"));
	}

	u32 quadVao, quadVbo;

	render::SceneFboInfo sceneFboInfo = render::setupSceneFbo(width, height);
	render::setupQuadBuffers(quadVao, quadVbo);

	state.sceneTextureId = sceneFboInfo.colorTextureId;

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

		Timeline &timeline = Timeline::get();

		state.time = timeline.advanceTimestep(frameStart);
		handleInput(window, getCurrentInputState(), getLastInputState(), camera, delta);

		swapInputStates();

		updateCameraFront(camera);

		for (ResourceId &id : shaderRecompiles)
		{
			ShaderBank::get().recompileAndLink(id);
		}
		shaderRecompiles.clear();

		//
		// render
		//

		render::bindFrameBuffer(sceneFboInfo.fboId);

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

		for (const TimelineItem& item : timeline.items) {
			if (timeline.itemActive(item)) {
				if (item.type == TimelineItem::Type::Scene) {
					Scene *scene = SceneBank::get().find(item.id);
					if (scene) {
						renderModels(*scene, state, v, p);
					}
				}
			}
		}

		render::bindFrameBuffer(0);

		Gui::get().run(state);
		Gui::get().render();

		glfwSwapBuffers(window);
	}

	Gui::get().terminate();
	glfwDestroyWindow(window);

	FileWatcher::destroy();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


