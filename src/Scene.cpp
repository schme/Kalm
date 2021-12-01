#include "Scene.h"

#include "render/gl_model.h"
#include "main.h"

namespace ks {

Model *Scene::addModel(const std::string &name) {
	ModelManager &mm = ModelManager::get();
	Model *model = mm.find(name);
	if (!model)
		return nullptr;

	models.push_back(model);
    ModelRenderAttributes mra;
    render::setupModel(*model, mra);
	attributes.emplace_back(mra);
	return model;
}

}
