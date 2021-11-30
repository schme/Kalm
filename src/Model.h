#pragma once

#include <string>
#include <vector>

#include "include/maths.h"
#include "Mesh.h"

namespace ks
{
	struct Model {
		std::string name = "";
		math::vec3 position = math::vec3(0.f, 0.f, 0.f);
		math::vec3 scale = math::vec3(1.f, 1.f, 1.f);
		math::vec3 rotation = math::vec3(0.0f, 0.0f, 0.0f);
		std::vector<Mesh> meshes = {};
	};

}
