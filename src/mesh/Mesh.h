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
	int stride = 0;
	std::vector<BufferDescriptor> buffers = {};
};

struct Mesh {
	math::AABB bounds;
	MeshDescriptor descriptor;
	std::string name = "";
	std::vector<float> vertices;
	std::vector<unsigned> indices;
};

struct Model {
	std::string name = "";
	std::vector<Mesh> meshes = {};
};

}