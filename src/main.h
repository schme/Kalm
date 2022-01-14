#pragma once

#include "Camera.h"
#include "ResourceId.h"
#include "Scene.h"

namespace ks {

struct SimulationState {
};

struct EditorState {
	Camera camera;

	u32 sceneTextureId = 0;
	math::vec2 outputSize = math::vec2(1.f, 1.f);
	float time = 0.0f;
	bool renderWireframe = false;

	std::string projectRoot = "";
	std::string shaderPrefix = "shaders/";
	std::string assetPrefix = "assets/";

	std::vector<ResourceId> reloadMeshes;
};

static EditorState& getEditorState()
{
	static EditorState editorState;
	return editorState;
}

}
