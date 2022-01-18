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
	bool drawOnViewport = false;
	float time = 0.0f;
	int width = 1920;
	int height = 1080;
	bool renderWireframe = false;

	std::string projectRoot = "";
	std::string assetPrefix = "assets/";

	std::vector<ResourceId> reloadMeshes;
};

static EditorState& getEditorState()
{
	static EditorState editorState;
	return editorState;
}

}
