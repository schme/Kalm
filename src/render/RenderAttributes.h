#pragma once

#include "include/common.h"
#include "ResourceId.h"
#include "Material.h"
#include <cstddef>
#include <string>
#include <vector>

namespace ks {

struct MeshRenderAttributes {
	std::string name;
	u32 vbo, ebo, indexCount;
};

struct ModelRenderAttributes {
	std::string name;
    u32 vao;
	ResourceId material;
	std::vector<MeshRenderAttributes> attr;
};

}
