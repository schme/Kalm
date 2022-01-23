#pragma once

#include "Camera.h"
#include "ResourceId.h"
#include "Scene.h"

namespace ks {

struct SimulationState {
};

struct EditorState {
	Camera camera;

	// scene fbo info
	u32 fboId = 0;
	u32 colorTextureId = 0;
	u32 stencilDepthBufferId = 0;

	bool drawOnViewport = false;
	float time = 0.0f;
	int width = 1920;
	int height = 1080;
	int bufferWidth = 1920;
	int bufferHeight = 1080;
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
