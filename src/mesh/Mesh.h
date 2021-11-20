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
	math::vec3 position;
	math::vec3 scale;
	std::vector<float> vertices;
	std::vector<unsigned> indices;
};

struct Model {
	std::string name = "";
	math::vec3 position = math::vec3(0.f, 0.f, 0.f);
	math::vec3 scale = math::vec3(1.f, 1.f, 1.f);
	std::vector<Mesh> meshes = {};
};

}
