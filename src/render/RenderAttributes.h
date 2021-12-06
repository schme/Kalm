#pragma once

#include "include/common.h"
#include "ResourceId.h"
#include <cstddef>
#include <string>
#include <vector>

namespace ks {

class Shader;
class Texture;

struct MeshRenderAttributes {
	std::string name;
	u32 vbo, ebo, shader, indexCount;
};

struct ModelRenderAttributes {
	std::string name;
    u32 vao;
	ResourceId shader;
    Texture *texture0 = nullptr;
    Texture *texture1 = nullptr;
    Texture *texture2 = nullptr;
    Texture *texture3 = nullptr;
	std::vector<MeshRenderAttributes> attr;
};

}
