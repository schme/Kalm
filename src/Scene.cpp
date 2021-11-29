#include "Scene.h"

#include "main.h"
#include "render/gl_model.h"

namespace ks {

Model *Scene::addModel(const std::string &name) {
	MeshManager &mm = MeshManager::get();
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

void Scene::renderModels(EditorState &state, const math::mat4 &view, const math::mat4 &perspective) {

	for (size_t i=0; i < models.size(); ++i) {

		const Model *model = models[i];
		ModelRenderAttributes &attribute = attributes[i];

		math::quat rotation = math::quat(model->rotation);
		math::mat4 m = math::mat4(1.0f);
		math::mat4 t = math::translate(m, model->position);
		math::mat4 r = math::toMat4(rotation);
		math::mat4 s = math::scale(m, model->scale);

		m = t * r * s;
		math::mat4 mvp = perspective * view * m;

        render::renderModel(state, attribute, mvp);
	}
}
}
