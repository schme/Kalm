#pragma once

#include "mesh/Mesh.h"
#include "mesh/MeshManager.h"
#include "mesh/MeshGenerators.h"
#include "render/RenderAttributes.h"

#include <vector>

namespace ks {
struct EditorState;

struct Scene {

	std::string name = "";
	std::vector<Model*> models;
	std::vector<ModelRenderAttributes> attributes;

	bool addModel(const std::string &name);

    void setupModel(Model &model, ModelRenderAttributes &attribute);
	void setupModels();
	void renderModels(EditorState &state, const math::mat4 &view, const math::mat4 &perspective, float time);
};

}
