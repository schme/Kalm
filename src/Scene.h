#pragma once

#include "Mesh.h"
#include "MeshManager.h"
#include "MeshGenerators.h"
#include "render/RenderAttributes.h"

#include <vector>

namespace ks {
struct EditorState;

struct Scene {

	std::string name = "";
	std::vector<Model*> models;
	std::vector<ModelRenderAttributes> attributes;

	Model *addModel(const std::string &name);

    void setupModel(Model &model, ModelRenderAttributes &attribute);
	void setupModels();
	void renderModels(EditorState &state, const math::mat4 &view, const math::mat4 &perspective);
};

}
