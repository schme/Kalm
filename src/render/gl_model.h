#pragma once


#include "main.h"
#include "Mesh.h"
#include "gl_shader.h"
#include "RenderAttributes.h"
#include "Rendering.h"
#include "Texture.h"
#include "render/glRendering.h"


namespace ks::render {
static float quadVertices[] = {  
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};	

struct SceneFboInfo {
	u32 fboId = 0;
	u32 colorTextureId = 0;
	u32 stencilDepthBufferId = 0;
};

static SceneFboInfo setupSceneFbo(u32 width, u32 height)
{
	SceneFboInfo info;
	info.fboId = generateFrameBuffer();
	bindFrameBuffer(info.fboId);

	info.colorTextureId = generateTexture();
	loadTexture(info.colorTextureId, width, height, 3, nullptr, false);
	setFramebufferAttachment(info.colorTextureId);

	glGenRenderbuffers(1, &info.stencilDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, info.stencilDepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER,
			GL_DEPTH24_STENCIL8_EXT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, info.stencilDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	assert(checkFramebufferStatus());

	bindFrameBuffer(0);
	return info;
}

static void setupQuadBuffers(u32 &quadVao, u32 &quadVbo)
{
	glGenVertexArrays(1, &quadVao);
	glGenBuffers(1, &quadVbo);

	glBindVertexArray(quadVao);

	glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	u32 vPosLocation, vTexLocation;

	vPosLocation = VertexAttribLocation::Position;
	vTexLocation = VertexAttribLocation::Texcoord0;

	u32 stride = (2 + 2) * sizeof(float);

	glEnableVertexAttribArray(vPosLocation);
	glVertexAttribPointer(vPosLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(vTexLocation);
	glVertexAttribPointer(vTexLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

static void renderSceneFboToScreen(const SceneFboInfo &info, u32 quadVao)
{
	clearColorBuffer(math::vec4(1.0f, 0.50f, 0.10f, 1.f));

	ShaderManager::get().find("pass")->use();

	glBindVertexArray(quadVao);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, info.colorTextureId);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void setupModel(const Model &model, ModelRenderAttributes &mra)
{
	if (!mra.shader) {
		mra.shader = ShaderManager::get().find("default");
	}

	Shader &shader = *mra.shader;

	GLuint vbo, vao, ebo;
	GLint vPosLocation, vColLocation, vNormLocation;
    GLint vTex0Location, vTex1Location, vTex2Location;

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

		vPosLocation = VertexAttribLocation::Position;
		vNormLocation = VertexAttribLocation::Normal;
		vColLocation = VertexAttribLocation::Color;
		vTex0Location = VertexAttribLocation::Texcoord0;
		vTex1Location = VertexAttribLocation::Texcoord1;
		vTex2Location = VertexAttribLocation::Texcoord2;

		const MeshDescriptor &md = mesh.descriptor;
		for (const BufferDescriptor &bd : md.buffers) {
            switch(bd.type) {
                case BufferType::Vertex: {
                     glEnableVertexAttribArray(vPosLocation);
                     glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE,
                             md.stride, reinterpret_cast<void*>(bd.offset));
                     break;
                }
                case BufferType::Color: {

                    glEnableVertexAttribArray(vColLocation);
                    glVertexAttribPointer(vColLocation, 4, GL_FLOAT, GL_FALSE,
                    md.stride, reinterpret_cast<void*>(bd.offset));
                    break;
                }
                case BufferType::Normal: {

                    glEnableVertexAttribArray(vNormLocation);
                    glVertexAttribPointer(vNormLocation, 3, GL_FLOAT, GL_FALSE,
                     md.stride, reinterpret_cast<void*>(bd.offset));
                    break;
                }
                case BufferType::Texcoord0: {

                    glEnableVertexAttribArray(vTex0Location);
                    glVertexAttribPointer(vTex0Location, 2, GL_FLOAT, GL_FALSE,
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

inline void setupFrameRenderModels()
{
	glEnable(GL_DEPTH_TEST);
}

inline void renderModel(EditorState &state, ModelRenderAttributes &mra, math::mat4 &mvp)
{

    glBindVertexArray(mra.vao);

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

	static u32 ___txtr_id= 0;
    if (mra.texture0) {
        render::bindTexture(mra.texture0->id);
		___txtr_id = mra.texture0->id;
    }
    else {
        render::bindTexture(___txtr_id);
        //render::bindTexture(0);
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
