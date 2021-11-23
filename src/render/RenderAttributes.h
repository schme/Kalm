#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace ks {

class Shader;

struct MeshRenderAttributes {
	std::string name;
	unsigned vao, vbo, ebo, shader;
	size_t indexCount;
};

struct ModelRenderAttributes {
	std::string name;
	std::vector<MeshRenderAttributes> attr;
	Shader *shader = nullptr;
};

}
