#pragma once

#include <string>
#include <vector>

#include "include/maths.h"
#include "include/common.h"
#include "Mesh.h"
#include "Material.h"

namespace ks
{

class Texture;

struct Model {
	std::string name = "";
	math::vec3 position = math::vec3(0.f, 0.f, 0.f);
	math::vec3 scale = math::vec3(1.f, 1.f, 1.f);
	math::vec3 rotation = math::vec3(0.0f, 0.0f, 0.0f);
	ResourceId material = ResourceId("Matcap");
	std::vector<Mesh> meshes = {};
	bool is2D = false;
};

}
