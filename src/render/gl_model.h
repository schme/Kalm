#pragma once


#include "main.h"
#include "Mesh.h"
#include "gl_shader.h"
#include "RenderAttributes.h"
#include "Rendering.h"
#include "Texture.h"


namespace ks::render {

static void setupModel(const Model &model, ModelRenderAttributes &mra)
{
	if (!mra.shader) {
		mra.shader = ShaderManager::get().find("default");
	}

	Shader &shader = *mra.shader;

	GLuint vbo, vao, ebo;
	GLint vpos_location, vcol_location, vnorm_location;
    GLint tex0_location, tex1_location, tex2_location;

    mra.texture0 = model.texture0;

	mra.name = model.name;

	glGenVertexArrays(1, &vao);
    mra.vao = vao;

	for (const Mesh &mesh : model.meshes) {
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		// 1) bind vao, 2) bind and set vertex buffers 3) configure vertex attributes
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(float), mesh.indices.data(), GL_STATIC_DRAW);

		vpos_location = render::VertexAttribLocation::Position;
		vnorm_location = render::VertexAttribLocation::Normal;
		vcol_location = render::VertexAttribLocation::Color;
		tex0_location = render::VertexAttribLocation::Texcoord0;
		tex1_location = render::VertexAttribLocation::Texcoord1;
		tex2_location = render::VertexAttribLocation::Texcoord2;

		const MeshDescriptor &md = mesh.descriptor;
		for (const BufferDescriptor &bd : md.buffers) {
            switch(bd.type) {
                case BufferType::Vertex: {
                     glEnableVertexAttribArray(vpos_location);
                     glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                             md.stride, reinterpret_cast<void*>(bd.offset));
                     break;
                }
                case BufferType::Color: {

                    glEnableVertexAttribArray(vcol_location);
                    glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
                    md.stride, reinterpret_cast<void*>(bd.offset));
                    break;
                }
                case BufferType::Normal: {

                    glEnableVertexAttribArray(vnorm_location);
                    glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_FALSE,
                     md.stride, reinterpret_cast<void*>(bd.offset));
                    break;
                }
                case BufferType::Texcoord0: {

                    glEnableVertexAttribArray(tex0_location);
                    glVertexAttribPointer(tex0_location, 2, GL_FLOAT, GL_FALSE,
                        md.stride, reinterpret_cast<void*>(bd.offset));
                    break;
                }
                default: {
                    fprintf(stderr, "Unhandled BufferType\n");
                    break;
                }
            }
        }
        mra.attr.emplace_back(MeshRenderAttributes{mesh.name, vbo, ebo, shader.get(),
                static_cast<u32>(mesh.indices.size())});
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

    glBindVertexArray(mra.vao);

    if (mra.texture0) {
        render::bindTexture(mra.texture0->id);
    }
    else {
        render::bindTexture(0);
    }

	for (const auto &attr: mra.attr) {

		glBindBuffer(GL_ARRAY_BUFFER, attr.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attr.ebo);

		mra.shader->use();
		mra.shader->bind("MVP", mvp);
		mra.shader->bind("time", state.time);

		glDrawElements(GL_TRIANGLES, attr.indexCount, GL_UNSIGNED_INT, 0);
	}
}

}
