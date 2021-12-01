#pragma once


#include "main.h"
#include "Mesh.h"
#include "gl_shader.h"
#include "RenderAttributes.h"


namespace ks::render {

static void setupModel(const Model &model, ModelRenderAttributes &mra)
{
	if (!mra.shader) {
		mra.shader = ShaderManager::get().find("default");
	}

	assert(mra.shader && "No shader was found for model");

	Shader &shader = *mra.shader;

	GLuint vbo, vao, ebo;
	GLint vpos_location, vcol_location, vnorm_location, vuv_location;

	mra.name = model.name;

	glGenVertexArrays(1, &vao);

	for (const Mesh &mesh : model.meshes) {
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		// 1) bind vao, 2) bind and set vertex buffers 3) configure vertex attributes
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(float), mesh.indices.data(), GL_STATIC_DRAW);

		vpos_location = glGetAttribLocation(shader.get(), "vPos");
		vcol_location = glGetAttribLocation(shader.get(), "vCol");
		vnorm_location = glGetAttribLocation(shader.get(), "vNorm");
		vuv_location = glGetAttribLocation(shader.get(), "vUv");

		const MeshDescriptor &md = mesh.descriptor;
		for (const BufferDescriptor &bd : md.buffers) {
            switch(bd.type) {
                case BufferType::Vertex: {
                     glEnableVertexAttribArray(vpos_location);
                     glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                             md.stride, (void*)bd.offset);
                     break;
                }
                case BufferType::Color: {

                    glEnableVertexAttribArray(vcol_location);
                    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
                    md.stride, (void*)bd.offset);
                    break;
                }
                case BufferType::Normal: {

                    glEnableVertexAttribArray(vnorm_location);
                    glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_FALSE,
                     md.stride, (void*)bd.offset);
                    break;
                }
                case BufferType::Texcoord0: {

                    glEnableVertexAttribArray(vuv_location);
                    glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE,
                        md.stride, (void*)bd.offset);
                    break;
                }
                default: {
                    fprintf(stderr, "Unhandled BufferType\n");
                    break;
                }
            }
        }
        mra.attr.emplace_back(MeshRenderAttributes{mesh.name, vao, vbo, ebo, shader.get(), mesh.indices.size()});
	}
}

inline void renderModel(EditorState &state, ModelRenderAttributes &mra, math::mat4 &mvp)
{

	// TODO: yea this doesn't scale
	auto meshNameIt = state.reloadMeshes.begin();
	while (meshNameIt != state.reloadMeshes.end()) {
		auto &mm = ModelManager::get();
		Model *model = mm.find(mra.name);
		auto mesh = model->meshes.begin();
		bool found = false;
		while (mesh != model->meshes.end() && !found) {
			if (mesh->name.compare(*meshNameIt) == 0) {
                const MeshRenderAttributes *meshAttr;
                for (const MeshRenderAttributes &mera : mra.attr) {
                    if (mera.name.compare(mesh->name) == 0) {
                        meshAttr = &mera;
                    }
                }
                glBindBuffer(GL_ARRAY_BUFFER, meshAttr->vbo);
				glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(float), mesh->vertices.data(), GL_STATIC_DRAW);
				state.reloadMeshes.erase(meshNameIt);
				found = true;
			}
			mesh = std::next(mesh);
		}
		if (found) break;
	}

	for (const auto &attr: mra.attr) {

		glBindVertexArray(attr.vao);

		glBindBuffer(GL_ARRAY_BUFFER, attr.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attr.ebo);

		mra.shader->use();
		mra.shader->bind("MVP", mvp);
		mra.shader->bind("time", state.time);
		mra.shader->bind("maxVert", (float)attr.indexCount);

		glDrawElements(GL_TRIANGLES, attr.indexCount, GL_UNSIGNED_INT, 0);
	}
}

}
