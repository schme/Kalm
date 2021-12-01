#pragma once

#include "Camera.h"
#include "ResourceId.h"
#include "Scene.h"

namespace ks {

struct SimulationState {
};

struct EditorState {
	Scene scene;
	Camera camera;

	float time = 0.0f;

	bool renderWireframe = false;

	std::string projectRoot = "";
	std::vector<ResourceId> reloadMeshes;
};

}
