#pragma once

#include "Camera.h"
#include "Model.h"
#include "render/RenderAttributes.h"

#include <vector>

namespace ks {

struct Scene {

	std::string name = "";
	std::vector<Model*> models;
	std::vector<ModelRenderAttributes> attributes;
	std::vector<Camera> cameras;

	Model *addModel(const std::string &name);
};

}
