#pragma once

#include "Mesh.h"
#include "Model.h"

namespace ks {

enum class PrimitiveType {
	Cube,
	Quad,
	ScreenQuad,
};

static constexpr const char *toStr(PrimitiveType type)
{
	switch (type) {
		case PrimitiveType::Cube:
			return "Cube";
	}
	return "UnknownPrimitive";
}

static inline Model defaultCube()
{
	Model model;
	model.name = "Cube";

	Mesh cube;
	cube.bounds = {{-1, -1, -1}, {1, 1, 1}};
	cube.name = "Cube-Mesh";

	cube.vertices =
	{
-1.000000, 1.000000, -1.000000, 0.000000, 1.000000, -0.000000, 0.875000, 0.500000,
1.000000, 1.000000, 1.000000, 0.000000, 1.000000, -0.000000, 0.625000, 0.750000,
1.000000, 1.000000, -1.000000, 0.000000, 1.000000, -0.000000, 0.625000, 0.500000,
1.000000, 1.000000, 1.000000, 0.000000, -0.000000, 1.000000, 0.625000, 0.750000,
-1.000000, -1.000000, 1.000000, 0.000000, -0.000000, 1.000000, 0.375000, 1.000000,
1.000000, -1.000000, 1.000000, 0.000000, -0.000000, 1.000000, 0.375000, 0.750000,
-1.000000, 1.000000, 1.000000, -1.000000, 0.000000, 0.000000, 0.625000, 0.000000,
-1.000000, -1.000000, -1.000000, -1.000000, 0.000000, 0.000000, 0.375000, 0.250000,
-1.000000, -1.000000, 1.000000, -1.000000, 0.000000, 0.000000, 0.375000, 0.000000,
1.000000, -1.000000, -1.000000, 0.000000, -1.000000, 0.000000, 0.375000, 0.500000,
-1.000000, -1.000000, 1.000000, 0.000000, -1.000000, 0.000000, 0.125000, 0.750000,
-1.000000, -1.000000, -1.000000, 0.000000, -1.000000, 0.000000, 0.125000, 0.500000,
1.000000, 1.000000, -1.000000, 1.000000, -0.000000, 0.000000, 0.625000, 0.500000,
1.000000, -1.000000, 1.000000, 1.000000, -0.000000, 0.000000, 0.375000, 0.750000,
1.000000, -1.000000, -1.000000, 1.000000, -0.000000, 0.000000, 0.375000, 0.500000,
-1.000000, 1.000000, -1.000000, 0.000000, 0.000000, -1.000000, 0.625000, 0.250000,
1.000000, -1.000000, -1.000000, 0.000000, 0.000000, -1.000000, 0.375000, 0.500000,
-1.000000, -1.000000, -1.000000, 0.000000, 0.000000, -1.000000, 0.375000, 0.250000,
-1.000000, 1.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.875000, 0.750000,
-1.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.625000, 1.000000,
-1.000000, 1.000000, -1.000000, -1.000000, 0.000000, 0.000000, 0.625000, 0.250000,
1.000000, -1.000000, 1.000000, 0.000000, -1.000000, 0.000000, 0.375000, 0.750000,
1.000000, 1.000000, 1.000000, 1.000000, -0.000000, 0.000000, 0.625000, 0.750000,
1.000000, 1.000000, -1.000000, 0.000000, 0.000000, -1.000000, 0.625000, 0.500000,
	};

	cube.indices =
	{
0, 1, 2,
3, 4, 5,
6, 7, 8,
9, 10, 11,
12, 13, 14,
15, 16, 17,
0, 18, 1,
3, 19, 4,
6, 20, 7,
9, 21, 10,
12, 22, 13,
15, 23, 16,
	};

	cube.descriptor.stride = (3 + 3 + 2) * sizeof(float);
	cube.descriptor.buffers = {
		{BufferType::Vertex, 0},
		{BufferType::Normal, 3 * sizeof(float)},
		{BufferType::Texcoord0, (3 + 3) * sizeof(float)},
	};

	model.meshes.emplace_back(std::move(cube));

	return model;
}

static inline Model defaultQuad()
{
	Model model;
	model.name = "Quad";

	Mesh quad;
	quad.bounds = {{-1, -1, 0}, {1, 1, 0}};
	quad.name = "Quad-Mesh";

	quad.vertices =
	{
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	quad.indices =
	{
		0, 1, 2,
		0, 2, 3,
	};

	quad.descriptor.stride = (3 + 2) * sizeof(float);
	quad.descriptor.buffers = {
		{BufferType::Vertex, 0},
		{BufferType::Texcoord0, 3 * sizeof(float)},
	};

	model.meshes.emplace_back(std::move(quad));

	return model;
}

static Model createPrimitive(PrimitiveType type)
{
	if (type == PrimitiveType::Cube)
		return defaultCube();

	if (type == PrimitiveType::Quad)
		return defaultQuad();

	if (type == PrimitiveType::ScreenQuad) {
		Model quad = defaultQuad();
		quad.is2D = true;
		return quad;
	}

	return defaultCube();
}

	
}
