#include "ModelBank.h"

#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "include/common.h"
#include "main.h"

namespace ks {

static void fillMeshDescriptors(const aiMesh &mesh, MeshDescriptor &descriptor)
{
	descriptor.buffers.clear();
	int offset = 0;
	if (mesh.HasPositions()) {
		descriptor.buffers.push_back(
				BufferDescriptor{BufferType::Vertex, offset});
		offset += sizeof(float) * 3;
	}
	if (mesh.HasVertexColors(0)) {
		descriptor.buffers.push_back(
				BufferDescriptor{BufferType::Color, offset});
		offset += sizeof(float) * 4;
	}
	if (mesh.HasNormals()) {
		descriptor.buffers.push_back(
				BufferDescriptor{BufferType::Normal, offset});
		offset += sizeof(float) * 3;
	}
	if (mesh.HasTextureCoords(0)) {
		descriptor.buffers.push_back(
				BufferDescriptor{BufferType::Texcoord0, offset});
		offset += sizeof(float) * 2;
	}
	if (mesh.HasTextureCoords(1)) {
		descriptor.buffers.push_back(
				BufferDescriptor{BufferType::Texcoord1, offset});
		offset += sizeof(float) * 2;
	}
	if (mesh.HasTextureCoords(2)) {
		descriptor.buffers.push_back(
				BufferDescriptor{BufferType::Texcoord2, offset});
		offset += sizeof(float) * 2;
	}
	descriptor.stride = offset;
}

static void fillVertex(const BufferDescriptor &desc, const int stride, const aiMesh &mesh, std::vector<float> &out)
{
	int offset = desc.offset / sizeof(float);
	for (unsigned i=0; i < mesh.mNumVertices; ++i) {
		auto vert = mesh.mVertices[i];
		out[offset] = vert.x;
		out[offset + 1] = vert.y;
		out[offset + 2] = vert.z;
		offset += stride;
	}
}
static void fillColors(const BufferDescriptor &desc, const int stride, const aiMesh &mesh, std::vector<float> &out)
{
	int offset = desc.offset / sizeof(float);
	for (unsigned i=0; i < mesh.mNumVertices; ++i) {
		auto vert = mesh.mColors[0][i];
		out[offset] = vert.r;
		out[offset + 1] = vert.g;
		out[offset + 2] = vert.b;
		out[offset + 3] = vert.a;
		offset += stride;
	}
}
static void fillNormals(const BufferDescriptor &desc, const int stride, const aiMesh &mesh, std::vector<float> &out)
{
	int offset = desc.offset / sizeof(float);
	for (unsigned i=0; i < mesh.mNumVertices; ++i) {
		auto vert = mesh.mNormals[i];
		out[offset] = vert.x;
		out[offset + 1] = vert.y;
		out[offset + 2] = vert.z;
		offset += stride;
	}
}
static void fillTexcoords(const BufferDescriptor &desc, const int stride, const int index, const aiMesh &mesh, std::vector<float> &out)
{
	int offset = desc.offset / sizeof(float);
	for (unsigned i=0; i < mesh.mNumVertices; ++i) {
		auto vert = mesh.mTextureCoords[index][i];
		out[offset] = vert.x;
		out[offset + 1] = vert.y;
		offset += stride;
	}
}

static void importMesh(const aiScene *scene, unsigned index, Model &model)
{
	MeshDescriptor descriptor;
	Mesh out = {};

	const aiMesh &mesh = *scene->mMeshes[index];
	out.name = mesh.mName.C_Str();

    if (mesh.mPrimitiveTypes != aiPrimitiveType_TRIANGLE) {
        log_warning("%s", "Ignoring non-triangle type on mesh import\n");
    }

	fillMeshDescriptors(mesh, descriptor);
	out.descriptor = descriptor;

	int numVertices = mesh.mNumVertices * descriptor.stride / sizeof(float);

	out.vertices.resize(numVertices);

	int elemStride = descriptor.stride / sizeof(float);

	for (const BufferDescriptor &desc : descriptor.buffers) {
		// assumes all possible elements are the size of a float
		switch (desc.type) {
			case BufferType::Vertex: {
				fillVertex(desc, elemStride, mesh, out.vertices);
				break;
			}
			case BufferType::Color: {
				fillColors(desc, elemStride, mesh, out.vertices);
				break;
			}
			case BufferType::Normal: {
				fillNormals(desc, elemStride, mesh, out.vertices);
				break;
			}
			case BufferType::Texcoord0: {
				fillTexcoords(desc, elemStride, 0, mesh, out.vertices);
				break;
			}
			case BufferType::Texcoord1: {
				fillTexcoords(desc, elemStride, 1, mesh, out.vertices);
				break;
			}
			case BufferType::Texcoord2: {
				fillTexcoords(desc, elemStride, 2, mesh, out.vertices);
				break;
			}
            default: break;
		}
	}

	out.indices.clear();
	if (mesh.HasFaces()) {
		out.indices.resize(mesh.mNumFaces * 3);

		int offset = 0;
		for (unsigned i=0; i < mesh.mNumFaces; ++i) {
			auto face = mesh.mFaces[i];
			assert (face.mNumIndices == 3);
			for (unsigned j=0; j < face.mNumIndices; ++j) {
				out.indices[offset] = face.mIndices[j];
				++offset;
			}
		}
	}
	out.bounds = math::AABB{
		{mesh.mAABB.mMin.x, mesh.mAABB.mMin.y, mesh.mAABB.mMin.z},
		{mesh.mAABB.mMax.x, mesh.mAABB.mMax.y, mesh.mAABB.mMax.z}};

    log_trace("Loaded mesh { %s }\n", toStr(out).c_str());
	model.meshes.push_back(std::move(out));
}

void ModelBank::importScene(const aiScene *scene, Model &model)
{
	if (scene->HasMeshes())	{
		for (unsigned i=0; i < scene->mNumMeshes; ++i) {
			importMesh(scene, i, model);
		}
	}
}

Model* ModelBank::load(const std::string &filepath, bool absolutePath)
{
	std::string fp = absolutePath ? filepath : getEditorState().modelPrefix + filepath;
	if (readFile(fp))
	{
		removeExtension(fp);
		return ResourceStorage<Model>::get().find(fp);
	}
	return nullptr;
}

bool ModelBank::readFile(const std::string &filename)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename,
			aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_SortByPType);

	if (!scene) {
		fprintf(stderr, "Mesh load failed: %s\n", importer.GetErrorString());
		return false;
	}

	fprintf(stderr,
			"Loaded file: %s\n"
			"Found %d meshes, %d materials, %d animations, %d textures, %d lights and %d cameras.\n"
			, filename.c_str(), scene->mNumMeshes, scene->mNumMaterials, scene->mNumAnimations, scene->mNumTextures,
			scene->mNumLights, scene->mNumCameras);

	fprintf(stderr, "Importing...\n");

	Model model = {};

	if (scene->HasMeshes()) {
		for (unsigned i=0; i < scene->mNumMeshes; ++i) {
			importMesh(scene, i, model);
		}
	}

	std::string sceneName = scene->mName.length == 0 ? filename : scene->mName.C_Str();
	removeExtension(sceneName);
	model.name = sceneName;
	ResourceStorage<Model>::get().add(sceneName, std::move(model));

	return true;
}

Model * ModelBank::addModel(Model &&model)
{
	auto& storage = ResourceStorage<Model>::get();
	Model *m = storage.find(model.name);

	if (!m) {
		m = storage.add(model.name, std::move(model));
	}
	return m;
}

Model * ModelBank::addModel(Model &model)
{
	auto& storage = ResourceStorage<Model>::get();
	Model *m = storage.find(model.name);

	if (!m) {
		m = storage.add(model.name, std::move(model));
	}
	return m;
}

Model * ModelBank::addModelRename(Model &&model)
{
	std::string newName;

	Model *addedMesh = addModel(model);
	int counter = 0;
	while (!addedMesh) {
		newName = model.name + std::to_string(counter);
        model.name = newName;
		addedMesh = addModel(model);
	}
	return addedMesh;
}

Model *ModelBank::addPrimitive(PrimitiveType type)
{
	Model primitive = createPrimitive(type);
	Model *model = addModelRename(std::move(primitive));
	return model;
}

}
