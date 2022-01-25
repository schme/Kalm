#pragma once

#include "Camera.h"
#include "Model.h"
#include "render/RenderAttributes.h"

#include <vector>
#include <string>

namespace ks {

struct Scene {

	Model *addModel(const std::string &name);

	Camera camera = {};
	std::string name = "";
	std::vector<Model*> models;
	std::vector<ModelRenderAttributes> attributes;
	std::vector<Camera> cameras;
};

}
