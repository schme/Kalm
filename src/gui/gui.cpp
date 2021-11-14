#include "gui.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ks {

bool Gui::init(GLFWwindow *window, int currentFrame, int maxFrames, float playbackRate)
{
	const char *glsl_version = "#version 460";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// imgui settings
	{
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigDockingWithShift = true;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	}

	if (!timelineGui) {
		timelineGui = std::make_unique<TimelineGui>(new TimelineGui);

		timelineGui->frameMax = maxFrames;
		timelineGui->currentFrame = currentFrame;

		timelineGui->items.push_back({0, 2, 0});
		timelineGui->items.push_back({10, 18, 0});
		timelineGui->items.push_back({0, 30, 0});
		timelineGui->items.push_back({42, 58, 0});

		timelineGui->sequenceOptions =
			ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_EDIT_ALL
			| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_ADD
			| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_DEL
			| ImSequencer::SEQUENCER_OPTIONS::SEQUENCER_COPYPASTE
		;

	}

	return true;
}

static void showMainMenuBar(Gui &gui)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
			ImGui::ColorEdit3("clear color", (float*)&gui.clear_color); // Edit 3 floats representing a color
            ImGui::EndMenu();
        }

		ImGui::Separator();
		ImGui::Checkbox("Demo Window", &gui.optShowDemoWindow);
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::EndMainMenuBar();
	}
}

static void showTimeline(Gui &gui)
{
	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoCollapse
	;

	ImGui::Begin("Timeline", nullptr, windowFlags);
	TimelineGui &timeline = *gui.timelineGui;
	auto sequencer = reinterpret_cast<ImSequencer::SequenceInterface*>(gui.timelineGui.get());
	ImSequencer::Sequencer(sequencer, &timeline.currentFrame, &timeline.expanded, &timeline.selectedEntry, &timeline.firstFrame, timeline.sequenceOptions);

	ImGui::End();
}

void Gui::run()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),
			ImGuiDockNodeFlags_PassthruCentralNode
			| ImGuiDockNodeFlags_NoDockingInCentralNode);

	showMainMenuBar(*this);

	if (optShowTimeline)
		showTimeline(*this);

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
