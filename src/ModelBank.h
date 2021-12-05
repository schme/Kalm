#pragma once

#include "Mesh.h"
#include "MeshGenerators.h"
#include "Model.h"
#include "include/maths.h"
#include "ResourceBank.h"

#include <map>
#include <vector>
#include <string>

class aiScene;
class aiNode;
class aiMesh;

namespace ks {

class ModelBank : public ResourceBank<Model, ModelBank> {
public:

    Model* load(const std::string &filepath, bool absolutePath = false) override;

	// true on success
	bool readFile(const std::string &filename);

	Model * addModel(Model &&model);
	Model * addModel(Model &model);
	Model * addPrimitive(PrimitiveType type);
	Model * addModelRename(Model &&model);


private:

	void importScene(const aiScene *scene, Model &model);
};


}
