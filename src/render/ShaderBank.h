#pragma once

#include "ResourceBank.h"
#include "gl_shader.h"

namespace ks {

struct ShaderBank : public ResourceBank<Shader, ShaderBank>
{
	Shader* createDefault() {
		Shader* shader = create("default");
        bool res = false;
		shader
			->attach(default_vert, Shader::Type::Vert, res)
			->attach(default_frag, Shader::Type::Frag, res)
			->link();

		return shader;
	}

	Shader* createPass() {
		Shader* shader = create("pass");
		bool res = false;
		shader
			->attach(pass_vert, Shader::Type::Vert, res)
			->attach(pass_frag, Shader::Type::Frag, res)
			->link();

		return shader;
	}

	Shader* createMatcap() {
		Shader* shader = create("matcap");
		bool res = false;
		shader
			->attach(matcap_vert, Shader::Type::Vert, res)
			->attach(matcap_frag, Shader::Type::Frag, res)
			->link();
		return shader;
	}

	Shader* create(const std::string &name) {
		Shader* shader = find(name);
		if (!shader) {
			auto& storage = ResourceStorage<Shader>::get();
			shader = storage.add(ResourceId(name), Shader{});
			shader->create();
		}
		return shader;
	}

    Shader* load(const std::string &filepath, bool absolutePath) {
		ks_unused(absolutePath);
		return find(ResourceId(filepath));
	}
};

}
