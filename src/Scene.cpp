#include "Scene.h"

#include "main.h"
#include "render/gl_model.h"

namespace ks {

Model *Scene::addModel(const std::string &name) {
	ModelManager &mm = ModelManager::get();
	Model *model = mm.find(name);
	if (!model)
		return nullptr;

	models.push_back(model);
    ModelRenderAttributes mra;
    setupModel(*model, mra);
	attributes.emplace_back(mra);
	return model;
}

void Scene::setupModel(Model &model, ModelRenderAttributes &attribute)
{
    render::setupModel(model, attribute);
}

void Scene::setupModels() {
	for(size_t i=0; i < models.size(); ++i) {
		Model *model = models[i];
		ModelRenderAttributes &attribute = attributes[i];
		this->setupModel(*model, attribute);
	}
}

}
