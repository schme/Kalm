#pragma once

#include "Mesh.h"
#include "MeshGenerators.h"
#include "Model.h"
#include "include/maths.h"

#include <map>
#include <vector>
#include <string>

class aiScene;
class aiNode;
class aiMesh;

namespace ks {

class ModelManager {
public:

	void init(const std::string &defaultMeshPath);

	// true on success
	bool readFile(const std::string &filename);

	Model * addMesh(Model &&model);
	Model * addMesh(Model &model);

	Model * addPrimitive(PrimitiveType type);
	// Add numbers after name if name conflicts
	Model * addMeshRename(Model &&model);


	Model *find(const std::string &name) {
		auto it = models.find(name);
		if (it != models.end()) {
			return &it->second;
		}
		return nullptr;
	}

	std::string defaultMeshPath = "";

	static ModelManager &get() {
		static ModelManager m;
		return m;
	}

private:

	void importScene(const aiScene *scene, Model &model);

	std::map<std::string, Model> models = {};
};


}
