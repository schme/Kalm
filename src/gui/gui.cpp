#include "gui.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "ResourceBank.h"
#include "Texture.h"
#include "Timeline.h"
#include "gui/FileBrowser.h"
#include "include/common.h"
#include "main.h"
#include "render/Rendering.h"
#include "render/gl_shader.h"


#include <sstream>

namespace ks {

static ImGuiDockNodeFlags windowFlags = ImGuiDockNodeFlags_PassthruCentralNode;

constexpr static bool isWidthBound(float outerAspect, float innerAspect)
{
	return outerAspect <= innerAspect;
}

static ImVec2 fitRectInRectKeepAspect(ImVec2 outerSize, float innerAspect)
{
	float outerAspect = outerSize.x / outerSize.y;
	if (isWidthBound(outerAspect, innerAspect))
		return ImVec2(outerSize.x, outerSize.x / innerAspect);

	return ImVec2(innerAspect * outerSize.y, outerSize.y);
}

bool Gui::init(GLFWwindow *window)
{
	const char *glsl_version = "#version 460";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	{
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigDockingWithShift = true;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	return true;
}

static void showMainMenuBar(Gui &gui, EditorState &state)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::Checkbox("Output Preview", &gui.optShowTimelinePreview);
			ImGui::Checkbox("Scene Window", &gui.optShowSceneWindow);
			ImGui::Checkbox("Editor Camera", &gui.optShowCameraWindow);
			ImGui::Checkbox("Texture Window", &gui.optShowTextureWindow);
			ImGui::Checkbox("Shaders Window", &gui.optShowShadersWindow);
			ImGui::Checkbox("Timeline Window", &gui.optShowTimeline);

			ImGui::Checkbox("Demo Window", &gui.optShowDemoWindow);
            ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Rendering"))
		{
			ImGui::Checkbox("Wireframe", &state.renderWireframe);
			ImGui::EndMenu();
		}
        if (ImGui::BeginMenu("Settings"))
        {
            ImGui::EndMenu();
        }

		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::EndMainMenuBar();
	}
}

static void showTimelineWindow(Gui &gui, bool &opt)
{
	if (!opt)
		return;

	ImGui::Begin("Timeline", &opt, windowFlags);
	TimelineGui &timeline = Timeline::get();
	ImGui::InputInt("Max Frames", &timeline.frameMax);

	ImGui::InputFloat("Playback Rate", &timeline.playbackRate);
	ImGui::SameLine(); ImGui::Text("Current: %d", timeline.currentFrame);

	auto sequencer = reinterpret_cast<ImSequencer::SequenceInterface*>(&timeline);
	ImSequencer::Sequencer(sequencer, &timeline.currentFrame, &timeline.expanded, &timeline.selectedEntry, &timeline.firstFrame, timeline.sequenceOptions);

	ImGui::End();
}

static void drawCameraWindow(Camera &camera, bool &opt)
{
	if (!opt)
		return;

	if (ImGui::Begin("Camera", &opt)) {
		ImGui::InputFloat3("position", math::value_ptr(camera.position));
		ImGui::InputFloat3("target", math::value_ptr(camera.target));
		ImGui::InputFloat3("up", math::value_ptr(camera.up));
		ImGui::InputFloat3("front", math::value_ptr(camera.front));
		ImGui::InputFloat3("right", math::value_ptr(camera.right));

		ImGui::InputFloat("fov", &camera.lens.fov, 0.f, 360.f, "%.3f");
		ImGui::InputFloat("aspect", &camera.lens.aspect, 0.f, 3.f, "%.3f");
		ImGui::InputFloat("near", &camera.lens.near, -1000.f, 1000.f, "%.3f");
		ImGui::InputFloat("far", &camera.lens.far, -1000.f, 1000.f, "%.3f");

		ImGui::InputFloat("yaw", &camera.yaw, 0, 0, "%.3f");
		ImGui::InputFloat("pitch", &camera.pitch, 0, 0, "%.3f");
		ImGui::InputFloat("roll", &camera.roll, 0, 0, "%.3f");

		ImGui::End();
	}
}

void drawMesh(EditorState &state, Mesh &mesh)
{
	size_t stride = mesh.descriptor.stride / sizeof(float);
	if (ImGui::TreeNode(mesh.name.c_str())) {
		std::stringstream id(mesh.name);
		size_t vertCount = 0;
		for (size_t i=0; i < mesh.vertices.size(); i += stride) {
			std::stringstream vertId;
			vertId << id.str();
			vertId << "pos";
			vertId << vertCount;
			if (ImGui::InputFloat3(vertId.str().c_str(), &mesh.vertices[i])) {
				state.reloadMeshes.push_back(mesh.name);
			}
			++vertCount;
		}
		ImGui::TreePop();
	}
}

void drawModel(EditorState &state, Model &model)
{
	if (ImGui::TreeNode(model.name.c_str())) {
		ImGui::Text("Name:");
		ImGui::SameLine(); ImGui::Text("%s", model.name.c_str());

		ImGui::InputFloat3("position", math::value_ptr(model.position));
		ImGui::InputFloat3("scale", math::value_ptr(model.scale));
		ImGui::InputFloat3("rotation", math::value_ptr(model.rotation));

		for (Mesh &mesh : model.meshes) {
			drawMesh(state, mesh);
		}
		ImGui::TreePop();
	}
}

void addPrimitive(PrimitiveType type, Scene &scene)
{
    auto &mm = ModelManager::get();
    Model *primitive = mm.addPrimitive(type);
    scene.addModel(primitive->name);
}

void drawSceneWindow(EditorState &state, bool &opt)
{
	if (!opt)
		return;

	if (ImGui::Begin("Scene", &opt)) {
        if (ImGui::Button("Add cube")) {
            addPrimitive(PrimitiveType::Cube, state.scene);
        }
		for (Model *model : state.scene.models) {
			drawModel(state, *model);
		}
	}
	ImGui::End();
}

void drawTexturePreview(const Texture &txtr, bool &opt)
{
	ImGui::Begin("Texture Preview", &opt, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::Image((void*)txtr.id,
			ImVec2(txtr.width, txtr.height),
			ImVec2(0.0f, 1.0f),
			ImVec2(1.0f, 0.0f),
			ImColor(1.0f, 1.0f, 1.0f, 1.0f),
			ImColor(0.0f, 1.0f, 0.0f, 1.0f));
	ImGui::End();
}

void drawTextureWindow(EditorState &state, bool &opt)
{
	static bool texturePreview = false;
	static bool showingFileBrowser = false;
	static const Texture* inPreview = nullptr;

	std::string selectedFile;

	if (!opt)
		return;

	if (ImGui::Begin("Textures", &opt)) {
		if (ImGui::Button("Add texture")) {
			showingFileBrowser = true;
		}
		auto &tb = ResourceBank<Texture>::get();
		for (const auto &[id, txtr] : tb.storage) {
			ImGui::Text("Id: %s", id.c_str());
			if (ImGui::ImageButton((void*)txtr.id, ImVec2(txtr.width / 20.0f, txtr.height / 20.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImColor(1.0f, 1.0f, 1.0f, 1.0f), ImColor(0.0f, 0.3f, 0.0f, 1.0f))) {
				texturePreview = true;
				inPreview = &txtr;
                log_info("inPreview set to: %s at %p\n", id.c_str(), &txtr);
			}
		}
	}
	ImGui::End();

	if (texturePreview && inPreview)
		drawTexturePreview(*inPreview, texturePreview);

	bool browserSelectionMade = false;
	if (showingFileBrowser) {
		browserSelectionMade = showFileBrowser(selectedFile, &showingFileBrowser);
	}

	if (browserSelectionMade) {
		browserSelectionMade = false;

		Texture *txtr = Texture::load(selectedFile, true);
		txtr->id = render::generateTexture();
		render::loadTexture(txtr->id, txtr->width, txtr->height, txtr->channels, txtr->data);
	}
}

void drawShadersWindow(EditorState &state, bool &opt)
{
	if (!opt)
		return;

    if (ImGui::Begin("Shaders Window", &opt)) {
        auto &mm = ShaderManager::get();
        for (auto& [id, shader] : mm.storage) {
            ImGui::Text("shader: %s", id.c_str());
        }
    }
    ImGui::End();

}

void drawTimelinePreview(EditorState &state, bool &opt)
{
	if (!opt)
		return;

	if (ImGui::Begin("Timeline Preview", &opt))	{
		ImVec2 windowSize = ImGui::GetWindowContentRegionMax();
		ImVec2 imageSize = fitRectInRectKeepAspect(ImVec2(windowSize.x - 10.f, windowSize.y - 30.f), state.outputSize.x / state.outputSize.y);
		const ImGuiStyle &style = ImGui::GetStyle();

		ImGui::Image(reinterpret_cast<void*>(state.sceneTextureId),
			imageSize,
			ImVec2(0.0f, 1.0f),
			ImVec2(1.0f, 0.0f),
			ImColor(1.0f, 1.0f, 1.0f, 1.0f),
			ImColor(1.0f, 1.0f, 1.0f, 1.0f));
}
	ImGui::End();
}

void Gui::run(EditorState &state)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	showMainMenuBar(*this, state);
	showTimelineWindow(*this, optShowTimeline);
	drawCameraWindow(state.camera, optShowCameraWindow);
	drawSceneWindow(state, optShowSceneWindow);
	drawTextureWindow(state, optShowTextureWindow);
    drawShadersWindow(state, optShowShadersWindow);
	drawTimelinePreview(state, optShowTimelinePreview);


	if (optShowDemoWindow)
		ImGui::ShowDemoWindow(&optShowDemoWindow);
}

void Gui::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::terminate()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
}
