#include "MaterialBank.h"

namespace ks {


Material* MaterialBank::load(const std::string &filepath, bool absolutePath)
{
	Material *mat = find("filepath");
	if (!mat) {
		mat = ResourceStorage<Material>::get().add(filepath, Material());
	}

	return mat;
}

}
