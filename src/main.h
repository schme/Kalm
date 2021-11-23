#pragma once

#include "Camera.h"
#include "Scene.h"

namespace ks {

struct SimulationState {
};

struct EditorState {
	Scene scene;
	Camera camera;

	bool renderWireframe = false;

	std::vector<std::string> reloadMeshes;
};

}
