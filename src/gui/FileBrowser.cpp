#include "FileBrowser.h"

#include "imgui.h"

#include <filesystem>

namespace ks
{
bool showPathTree(const std::string &rootPath, std::string &outFile)
{
	auto dir = std::filesystem::directory_iterator(rootPath);
	static ImGuiTreeNodeFlags base_flags = 0u;
	ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	for (auto file : dir) {
		if (file.is_directory()) {
			if (ImGui::TreeNode(file.path().filename().c_str())) {
				if(showPathTree(file.path(), outFile)) {
					return true;
				}
				ImGui::TreePop();
			}
		}
		else {

			ImGuiTreeNodeFlags node_flags = base_flags | leaf_flags;

			const bool is_selected = outFile.compare(file.path()) == 0;
			if (is_selected)
				node_flags |= ImGuiTreeNodeFlags_Selected;

			ImGui::TreeNodeEx(file.path().filename().c_str(), node_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				if (is_selected) outFile = "";
				else outFile = file.path();
			}
		}
	}
	return false;
}

bool showFileBrowser(std::string &outFile, bool *opt)
{
	bool hasResult = false;
	static std::string currentSelection = "";
	static std::filesystem::path currentPath = std::filesystem::current_path();

	ImGui::Begin("File Browser", opt);

	showPathTree(currentPath, currentSelection);
	bool hasSelection = !currentSelection.empty();

	ImGui::BeginDisabled(!hasSelection);
	if (ImGui::Button("Open")) {
		*opt = false;
		outFile = currentSelection;
		hasResult = true;
	}
	ImGui::EndDisabled();
	ImGui::SameLine();
	if (ImGui::Button("Close")) {
		*opt = false;
	}

	ImGui::End();
	return hasResult;
}

}
