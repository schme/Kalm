#include "gui.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "MaterialBank.h"
#include "ModelBank.h"
#include "ResourceStorage.h"
#include "SceneBank.h"
#include "Texture.h"
#include "Timeline.h"
#include "gui/FileBrowser.h"
#include "include/common.h"
#include "main.h"
#include "render/Rendering.h"
#include "render/gl_shader.h"
#include "render/ShaderBank.h"

#include <sstream>

namespace ks {

static char guiInputBuffer[256];

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

	timeline.init(&Timeline::get());

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
			ImGui::Checkbox("Material Window", &gui.optShowMaterialWindow);

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
	ks_unused(gui);
	if (!opt)
		return;

	TimelineGui &tl = gui.timeline;

	ImGui::Begin("Timeline", &opt, windowFlags);
	ImGui::InputInt("Max Frames", &tl.timeline->frameMax);
	ImGui::SameLine(); ImGui::Text("Frame: %d", tl.timeline->currentFrame);
	ImGui::SameLine();
	if (ImGui::Button("Add Scenes")) {
		tl.Add(static_cast<int>(TimelineItem::Type::Scene));
	}

	auto sequencer = reinterpret_cast<ImSequencer::SequenceInterface*>(&tl);
	ImSequencer::Sequencer(sequencer, &tl.timeline->currentFrame, &tl.expanded, &tl.selectedEntry, &tl.timeline->firstFrame, tl.sequenceOptions);

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

		ImGui::SliderFloat("fov", &camera.lens.fov, 1.f, 180.f, "%.3f");
		ImGui::InputFloat("aspect", &camera.lens.aspect, 0.f, 3.f, "%.3f");
		ImGui::InputFloat("near", &camera.lens.near, -1000.f, 1000.f, "%.3f");
		ImGui::InputFloat("far", &camera.lens.far, -1000.f, 1000.f, "%.3f");

		ImGui::SliderFloat("camera.yaw", &camera.yaw, -179.9f, 179.9f);
		ImGui::SliderFloat("camera.pitch", &camera.pitch, -89.f, 89.f);
		ImGui::SliderFloat("camera.roll", &camera.roll, -179.9f, 179.9f);

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

void drawTexturePicker(ResourceId& selectedId, const char* name)
{
	const char* currentLabel = selectedId.c_str();
	if (ImGui::BeginCombo(name, currentLabel)) {
		auto& rs = ResourceStorage<Texture>::get();
		ImGui::PushID((void*)0);
		if (ImGui::Selectable("", selectedId == ""))
			selectedId = "";
		ImGui::PopID();
		for (const auto& [id, texture] : rs.storage) {
			ImGui::PushID((void*)&texture);
			if (ImGui::Selectable(id.c_str(), selectedId == id))
				selectedId = id;
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}
}

void drawShaderPicker(ResourceId& selectedId)
{
	const char* currentLabel = selectedId.c_str();
	if (ImGui::BeginCombo("Shader", currentLabel)) {
		auto& rs = ResourceStorage<Shader>::get();
		for (const auto& [id, shader] : rs.storage) {
			ImGui::PushID((void*)&shader);
			if (ImGui::Selectable(id.c_str(), selectedId == id))
				selectedId = id;
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}
}

void drawMaterialPicker(ResourceId& selectedId)
{
	const char* currentLabel = selectedId.c_str();
	if (ImGui::BeginCombo("Material", currentLabel)) {
		auto& ms = ResourceStorage<Material>::get();
		for (const auto& [id, material] : ms.storage) {
			ImGui::PushID((void*)&material);
			if (ImGui::Selectable(id.c_str(), selectedId == id))
				selectedId = id;
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}
}

void drawModel(EditorState &state, Model &model)
{
	if (ImGui::TreeNode(model.name.c_str())) {
		ImGui::Text("Name:");
		ImGui::SameLine(); ImGui::Text("%s", model.name.c_str());

		ImGui::Checkbox("hidden", &model.isHidden);
		ImGui::InputFloat3("position", math::value_ptr(model.position));
		ImGui::InputFloat3("scale", math::value_ptr(model.scale));
		ImGui::InputFloat3("rotation", math::value_ptr(model.rotation));

		drawMaterialPicker(model.material);

		for (Mesh &mesh : model.meshes) {
			drawMesh(state, mesh);
		}
		ImGui::TreePop();
	}
}

void addPrimitive(PrimitiveType type, Scene &scene)
{
	auto &mm = ModelBank::get();
	Model *primitive = mm.addPrimitive(type);
	scene.addModel(primitive->name);
}

static void  Strtrim(char* s) {
	char* str_end = s + strlen(s);
	while (str_end > s && str_end[-1] == ' ') str_end--;
	*str_end = 0;
}

void addNewEmptyScenePrompt()
{
	if (ImGui::BeginPopupModal("Add new empty scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::InputText("Scene name", guiInputBuffer, IM_ARRAYSIZE(guiInputBuffer),
					ImGuiInputTextFlags_EnterReturnsTrue))
		{
			ImGui::SetItemDefaultFocus();
			char *s = guiInputBuffer;
			Strtrim(s);
			if (s[0]) {
				SceneBank::get().load(std::string(s));
				std::strcpy(s, "");
				ImGui::CloseCurrentPopup();
			}
		}
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void drawScenePicker(ResourceId& selectedId)
{
	const char* currentLabel = selectedId.c_str();
	if (ImGui::BeginCombo("Scene List", currentLabel)) {
		auto& ss = ResourceStorage<Scene>::get();
		for (const auto& [id, scene] : ss.storage) {
			ImGui::PushID((void*)&scene);
			if (ImGui::Selectable(id.c_str(), selectedId == id))
				selectedId = id;
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}
}

void drawSceneWindow(EditorState &state, bool &opt)
{
	if (!opt)
		return;

	if (ImGui::Begin("Scene", &opt)) {

		addNewEmptyScenePrompt();

		if (ImGui::Button("Add scene")) {
			ImGui::OpenPopup("Add new empty scene");
		}

		auto &ss = ResourceStorage<Scene>::get();

		if (!ss.storage.empty()) {
			static ResourceId current = ss.storage.begin()->first;

			drawScenePicker(current);
			Scene *scene = ss.find(current);

			ImGui::Separator();

			if (ImGui::Button("Add Cube")) {
				addPrimitive(PrimitiveType::Cube, *scene);
			}
			if (ImGui::Button("Add Quad")) {
				addPrimitive(PrimitiveType::Quad, *scene);
			}
			if (ImGui::Button("Add ScreenQuad")) {
				addPrimitive(PrimitiveType::ScreenQuad, *scene);
			}
			for (Model *model : scene->models) {
				drawModel(state, *model);
			}
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
	ks_unused(state);
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
		auto &tb = ResourceStorage<Texture>::get();
		for (const auto &[id, txtr] : tb.storage) {
			ImGui::Text("Id: %s", id.c_str());
			if (ImGui::ImageButton((void*)txtr.id, ImVec2(txtr.width / 20.0f, txtr.height / 20.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImColor(1.0f, 1.0f, 1.0f, 1.0f), ImColor(0.0f, 0.3f, 0.0f, 1.0f))) {
				texturePreview = true;
				inPreview = &txtr;
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
	}
}

void drawShadersWindow(EditorState &state, bool &opt)
{
	ks_unused(state);
	if (!opt)
		return;

	if (ImGui::Begin("Shaders Window", &opt)) {
		if (ImGui::Button("Refresh Shaders")) {
			ShaderBank::get().init();
		}
		auto &mm = ResourceStorage<Shader>::get();
		for (auto& [id, shader] : mm.storage) {
			ImGui::Text("shader: %s", id.c_str());
			ImGui::SameLine();
			if (ImGui::Button(std::string("Recompile " + id).c_str())) {
				ShaderBank::get().recompileAndLink(id);
			}
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
		ImVec2 imageSize = fitRectInRectKeepAspect(ImVec2(windowSize.x - 10.f, windowSize.y - 30.f), (float)state.width / state.height);

		ImGui::Image(reinterpret_cast<void*>(state.sceneTextureId),
				imageSize,
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f),
				ImColor(1.0f, 1.0f, 1.0f, 1.0f),
				ImColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	ImGui::End();
}

void addNewMaterialPrompt()
{
	if (ImGui::BeginPopupModal("Add new material", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::InputText("Material name", guiInputBuffer, IM_ARRAYSIZE(guiInputBuffer),
					ImGuiInputTextFlags_EnterReturnsTrue))
		{
			ImGui::SetItemDefaultFocus();
			char *s = guiInputBuffer;
			Strtrim(s);
			if (s[0]) {
				MaterialBank::get().load(std::string(s));
				std::strcpy(s, "");
				ImGui::CloseCurrentPopup();
			}
		}
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void drawMaterialWindow(EditorState &state, bool &opt)
{
	ks_unused(state);
	if (!opt)
		return;

	if (ImGui::Begin("Material Window", &opt)) {

		addNewMaterialPrompt();

		if (ImGui::Button("Add material")) {
			ImGui::OpenPopup("Add new material");
		}

		auto& ms = ResourceStorage<Material>::get();

		if (!ms.storage.empty()) {
			static ResourceId current = ms.storage.begin()->first;

			drawMaterialPicker(current);
			Material *material = ms.find(current);

			ImGui::Separator();

			drawShaderPicker(material->shader);
			drawTexturePicker(material->texture0, "texture0");
			drawTexturePicker(material->texture1, "texture1");
			drawTexturePicker(material->texture2, "texture2");
		}

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
	drawMaterialWindow(state, optShowMaterialWindow);


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
