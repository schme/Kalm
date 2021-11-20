#pragma once

#include "mesh/Mesh.h"
#include "mesh/MeshManager.h"
#include "render/gl_model.h"

#include <vector>

namespace ks {

struct Scene {
	std::string name = "";
	std::vector<Model*> models;
	std::vector<ModelRenderAttributes> attributes;

	bool addModel(const std::string &name) {
		MeshManager &mm = MeshManager::get();
		Model *model = mm.find(name);
		if (!model)
			return false;

		models.push_back(model);
		attributes.emplace_back(ModelRenderAttributes());
		return true;
	}

	void setupModels() {
		for(size_t i=0; i < models.size(); ++i) {
			const Model *model = models[i];
			ModelRenderAttributes &attribute = attributes[i];
			setupModel(*model, attribute);
		}
	}

	void renderModels(const math::mat4 &view, const math::mat4 &perspective, float time) {

		for (size_t i=0; i < models.size(); ++i) {

			const Model *model = models[i];
			ModelRenderAttributes &attribute = attributes[i];

			math::mat4 m = math::mat4(1.0f);
			m = math::translate(m, model->position);
			m = math::scale(m, model->scale);

			math::mat4 mvp = perspective * view * m;

			renderModel(attribute, mvp, time);
		}
	}
};

}
