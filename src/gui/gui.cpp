#include "gui.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Timeline.h"

namespace ks {

bool Gui::init(GLFWwindow *window)
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
		| ImGuiWindowFlags_NoCollapse
        | ImGuiDockNodeFlags_PassthruCentralNode
	;

	ImGui::Begin("Timeline", nullptr, windowFlags);
	TimelineGui &timeline = Timeline::get();
	auto sequencer = reinterpret_cast<ImSequencer::SequenceInterface*>(&timeline);
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
