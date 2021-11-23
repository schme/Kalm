#pragma once

#include "include/maths.h"
#include "Mesh.h"
#include "MeshGenerators.h"


#include <map>
#include <vector>
#include <string>

class aiScene;
class aiNode;
class aiMesh;

namespace ks {

class MeshManager {
public:

	void init(const std::string &defaultMeshPath);

	// true on success
	bool readFile(const std::string &filename);

	// true on success, false if already exists
	Model * addMesh(const std::string &name, Model &&model);
	Model * addMesh(const std::string &name, Model &model);

	Model * addPrimitive(PrimitiveType type);
	// Add numbers after name if name conflicts
	Model * addMeshRename(const std::string &name, Model &&model);


	Model *find(const std::string &name) {
		auto it = models.find(name);
		if (it != models.end()) {
			return &it->second;
		}
		return nullptr;
	}

	std::string defaultMeshPath = "";

	static MeshManager &get() {
		static MeshManager m;
		return m;
	}

private:

	void importScene(const aiScene *scene, Model &model);

	std::map<std::string, Model> models = {};
};


}
