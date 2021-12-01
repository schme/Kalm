#pragma once

#include "include/common.h"
#include <cstddef>
#include <string>
#include <vector>

namespace ks {

class Shader;

struct MeshRenderAttributes {
	std::string name;
	u32 vao, vbo, ebo, shader;
	size_t indexCount;
};

struct ModelRenderAttributes {
	std::string name;
	std::vector<MeshRenderAttributes> attr;
	Shader *shader = nullptr;
};

}
