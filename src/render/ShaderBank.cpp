#include "ShaderBank.h"

#include "main.h"

#include <filesystem>
#include <fstream>

namespace ks {

Shader::Type shaderExtensionToType(std::filesystem::path ext)
{
	if (ext.compare(".vert") == 0) {
		return Shader::Type::Vert;
	}
	if (ext.compare(".tesc") == 0) {
		return Shader::Type::TessCtrl;
	}
	if (ext.compare(".tese") == 0) {
		return Shader::Type::TessEval;
	}
	if (ext.compare(".geom") == 0) {
		return Shader::Type::Geom;
	}
	if (ext.compare(".frag") == 0) {
		return Shader::Type::Frag;
	}
	if (ext.compare(".comp") == 0) {
		return Shader::Type::Comp;
	}

	return Shader::Type::Count;
}

void attachShaderIfFound(const std::filesystem::path &shaderPath, Shader *shader, Shader::Type type)
{
	std::ifstream shaderFile(shaderPath);
	std::string shaderString((std::istreambuf_iterator<char>(shaderFile)),
			(std::istreambuf_iterator<char>()));

	bool success = false;
	shader->attach(shaderString.c_str(), type, success);
}

void attachShadersWhenFound(const std::filesystem::path &shaderPath, Shader *shader, const std::filesystem::path &name)
{
	auto it = std::filesystem::directory_iterator(shaderPath);
	for (auto file : it) {
		if (file.path().stem().compare(name) == 0) {
			attachShaderIfFound(file, shader, shaderExtensionToType(file.path().extension()));
		}
	}
}

void ShaderBank::init()
{
	auto dir = std::filesystem::directory_iterator(getEditorState().projectRoot + getEditorState().shaderPrefix);

	std::vector<std::filesystem::path> added;

	for (auto file : dir) {
		if (file.is_regular_file()) {
			auto name = file.path().stem();
			if (added.end() != std::find(added.begin(), added.end(), name))
				continue;

			// Start only on vert for now
			if (file.path().extension().compare(".vert") != 0)
				continue;

			added.push_back(name);

			log_info("Building shader: %s\n", name.c_str());

			Shader* shader = create(name);
			std::filesystem::path rootDir = getEditorState().projectRoot + getEditorState().shaderPrefix;

			attachShadersWhenFound(rootDir, shader, name);

			shader->link();
		}
	}
}

}
