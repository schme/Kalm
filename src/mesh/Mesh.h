#pragma once

#include "include/maths.h"
#include <string>
#include <vector>

namespace ks {

enum class BufferType {
	Vertex,
	Color,
	Normal,
	Texcoord0,
	Texcoord1,
	Texcoord2,
};

struct BufferDescriptor {
	BufferDescriptor() = default;
	BufferDescriptor(BufferType type, int offset)
		: type(type), offset(offset) {}
	BufferType type;
	int offset;
};

struct MeshDescriptor {
	std::vector<BufferDescriptor> buffers = {};
	int stride = 0;
};

struct Mesh {
	std::string name = "";
	std::vector<float> vertices;
	std::vector<unsigned> indices;
	MeshDescriptor descriptor;
	math::AABB bounds;
};

struct Model {
	std::string name = "";
	std::vector<Mesh> meshes = {};
};

}
