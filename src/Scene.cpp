#include "Scene.h"

#include "main.h"
#include "render/gl_model.h"

namespace ks {

bool Scene::addModel(const std::string &name) {
	MeshManager &mm = MeshManager::get();
	Model *model = mm.find(name);
	if (!model)
		return false;

	models.push_back(model);
    ModelRenderAttributes mra;
    setupModel(*model, mra);
	attributes.emplace_back(mra);
	return true;
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

void Scene::renderModels(EditorState &state, const math::mat4 &view, const math::mat4 &perspective, float time) {

	for (size_t i=0; i < models.size(); ++i) {

		const Model *model = models[i];
		ModelRenderAttributes &attribute = attributes[i];

		math::mat4 m = math::mat4(1.0f);
		m = math::translate(m, model->position);
		m = math::scale(m, model->scale);

		math::mat4 mvp = perspective * view * m;

        render::renderModel(state, attribute, mvp, time);
	}
}
}
