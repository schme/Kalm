#pragma once

#include "ResourceBank.h"
#include "gl_shader.h"

namespace ks {

struct ShaderBank : public ResourceBank<Shader, ShaderBank>
{

	void init() override;

	void recompileAndLink(const ResourceId &id);

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

	static Shader::Type extensionToType(const std::string& extension);
};

}
