#pragma once

#include "Mesh.h"
#include "ModelManager.h"
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
};

}
