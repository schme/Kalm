#include "MaterialBank.h"

#include "TextureBank.h"

namespace ks {


Material* MaterialBank::load(const std::string &filepath, bool absolutePath)
{
	ks_unused(absolutePath);

	Material *mat = find(filepath);
	if (!mat) {
		mat = ResourceStorage<Material>::get().add(filepath, Material());
	}

	return mat;
}

void MaterialBank::loadResourcesIfNeeded(ResourceId material) {

	if (!isValid(material)) {
		auto& rs = ResourceStorage<Material>::get();
		for (const auto& [id, material] : rs.storage) {
			loadResourcesIfNeeded(id);
		}
		return;
	}

	Material *mat = find(material);

	if (!mat)
		return;

	auto &tb = TextureBank::get();

	if (isValid(mat->texture0) && !tb.isLoaded(mat->texture0)) {
		tb.load(mat->texture0);
	}
	if (isValid(mat->texture1) && !tb.isLoaded(mat->texture1)) {
		tb.load(mat->texture1);
	}
	if (isValid(mat->texture2) && !tb.isLoaded(mat->texture2)) {
		tb.load(mat->texture2);
	}
}

}
