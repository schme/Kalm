#include "gui.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace ks {

bool Gui::init(GLFWwindow *window)
{
	const char *glsl_version = "#version 460";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

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
}

void Gui::run()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	showMainMenuBar(*this);

	if (optShowDemoWindow)
		ImGui::ShowDemoWindow(&optShowDemoWindow);

	if (optShowTimeline)
		showTimeline(*this);

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
