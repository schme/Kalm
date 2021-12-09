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

			math::quat rotation = math::quat(math::radians(model->rotation));
			math::mat4 m = math::mat4(1.0f);
			math::mat4 t = math::translate(m, model->position);
			math::mat4 r = math::toMat4(rotation);
			math::mat4 s = math::scale(m, model->scale);

			m = t * r * s;
			ModelRenderAttributes &attribute = scene.attributes[i];
			render::renderModel(state, attribute, m, view, perspective);
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

	state.outputSize = math::vec2(width, height);
	render::setupEnvironment(glfwGetProcAddress);

	ShaderBank::get().createDefault();
	ShaderBank::get().createPass();
	ShaderBank::get().createMatcap();

	auto& mm = ModelBank::get();
	mm.init();
	mm.readFile(getEditorState().modelPrefix + "ico.dae");
	mm.readFile(getEditorState().modelPrefix + "HappyBuddha.obj");
	mm.readFile(getEditorState(). modelPrefix + "grenade.fbx");

	Model* prim = mm.addPrimitive(PrimitiveType::Cube);
	prim->scale = math::vec3(20.f, 20.f, 20.f);
	std::string cubeId = prim->name;

	auto& tl = TextureBank::get();

	tl.init();
	Texture *texture = tl.load(getEditorState().texturePrefix + "matcap.png");
	texture->id = render::generateTexture();
	render::loadTexture(texture->id, texture->width, texture->height, texture->channels, texture->data);

	texture = tl.load(getEditorState().texturePrefix + "grenade_Base_color.png");
	texture->id = render::generateTexture();
	render::loadTexture(texture->id, texture->width, texture->height, texture->channels, texture->data);

	Gui& gui = Gui::get();
	gui.init(window);

	Timeline& timeline = Timeline::get();
	timeline.init();

	SceneBank::get().load("Scene0");
	Scene& mainScene = *SceneBank::get().load("Main");

	Model* model = mainScene.addModel("ico");
	model->position = math::vec3(0.f, -3.f, -5.f);

	model = mainScene.addModel("HappyBuddha");
	model->position = math::vec3(0.f, 2.f, 5.f);

	mainScene.addModel(cubeId);

	model = mainScene.addModel("grenade");
	model->position = math::vec3(0.f, 0.f, -1.f);
	model->rotation = math::vec3(-90.f, 0.f, 0.f);
	model->material = ResourceId("grenade");


	u32 quadVao, quadVbo;

	render::SceneFboInfo sceneFboInfo = render::setupSceneFbo(width, height);
	render::setupQuadBuffers(quadVao, quadVbo);

	Material *matcap = MaterialBank::get().load("matcap");
	matcap->shader = ResourceId("matcap");
	matcap->texture0 = ResourceId("matcap.png");

	Material *grenade = MaterialBank::get().load("grenade");
	grenade->shader = ResourceId("default");
	grenade->texture0 = ResourceId("grenade_Base_color.png");

	ModelBank::get().find("grenade")->material = ResourceId("grenade");

	state.sceneTextureId = sceneFboInfo.colorTextureId;

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

		state.time = timeline.advanceTimestep(frameStart);
		handleInput(window, getCurrentInputState(), getLastInputState(), camera, delta);

		swapInputStates();

		updateCameraFront(camera);

		// render
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
			if (item.type == TimelineItem::Type::Scene) {
				if (item.frameStart <= timeline.currentFrame
						&& item.frameEnd >= timeline.currentFrame) {
					Scene *scene = SceneBank::get().find(item.id);
					renderModels(*scene, state, v, p);
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

	glfwTerminate();
	exit(EXIT_SUCCESS);
}


