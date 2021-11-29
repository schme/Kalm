#pragma once

#include "Camera.h"
#include "Scene.h"

namespace ks {

struct SimulationState {
};

struct EditorState {
	Scene scene;
	Camera camera;

	float time = 0.0f;

	bool renderWireframe = false;

	std::vector<std::string> reloadMeshes;
};

}
