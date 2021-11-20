#pragma once


#include "mesh/Mesh.h"
#include "gl_shader.h"
#include "glad/gl.h"

#include <cstddef>
#include <string>
#include <vector>

namespace ks {


static const char* vertex_shader_text =
	"#version 460\n"
	"uniform mat4 MVP;\n"
	"uniform float time;\n"
	"in vec3 vPos;\n"
	"in vec4 vCol;\n"
	"in vec3 vNorm;\n"
	"in vec2 uv;\n"
	"out vec4 color;\n"
	"out vec2 tex;\n"
	"void main()\n"
	"{\n"
	"    color = vec4(vNorm.x, vNorm.y, 0.5 + sin(time) / 2.0, vCol.a);\n"
	"    tex = uv;\n"
	"    gl_Position = MVP * vec4(vPos, 1.0);\n"
	"}\n";

static const char* fragment_shader_text =
	"#version 460\n"
	"uniform mat4 MVP;\n"
	"uniform float time;\n"
	"in vec4 color;\n"
	"in vec2 tex;\n"
	"out vec4 fragColor;\n"
	"void main()\n"
	"{\n"
	"    fragColor = color;\n"
	"}\n";


struct MeshRenderAttributes {
	std::string name;
	GLuint vao, vbo, ebo, shader;
	size_t indexCount;
};

struct ModelRenderAttributes {
	std::string name;
	std::vector<MeshRenderAttributes> attr;
	Shader shader;
};

static void setupModel(const Model &model, ModelRenderAttributes &mra)
{
	Shader &shader = mra.shader;
	shader
		.attach(vertex_shader_text, Shader::Type::Vert)
		.attach(fragment_shader_text, Shader::Type::Frag)
		.link();

	GLuint vbo, vao, ebo;
	GLint vpos_location, vcol_location, vnorm_location, uv_location;

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
		uv_location = glGetAttribLocation(shader.get(), "uv");

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

												glEnableVertexAttribArray(uv_location);
												glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE,
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

void renderModel(ModelRenderAttributes &mra, math::mat4 &mvp, float time)
{
	for (const auto &attr: mra.attr) {

		glBindVertexArray(attr.vao);

		glBindBuffer(GL_ARRAY_BUFFER, attr.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attr.ebo);

		mra.shader.use();
		mra.shader.bind("MVP", mvp);
		mra.shader.bind("time", time);

		glDrawElements(GL_TRIANGLES, attr.indexCount, GL_UNSIGNED_INT, 0);
	}
}

}
