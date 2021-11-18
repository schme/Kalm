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

	static MeshManager &get() {
		static MeshManager m;
		return m;
	}

	std::string defaultMeshPath = "";

private:

	void importScene(const aiScene *scene, Model &model);

	std::map<std::string, Model> models = {};
};

}
