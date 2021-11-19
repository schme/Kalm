#pragma once

#include "include/maths.h"
#include "mesh/Mesh.h"

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
	bool readFile(const std::string &filename);

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
