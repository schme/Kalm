#pragma once

#include "Material.h"
#include "ResourceBank.h"

namespace ks {

struct MaterialBank : public ResourceBank<Material, MaterialBank> {

    Material* load(const std::string &filepath, bool absolutePath = false) override;

	void loadResourcesIfNeeded(const ResourceId material = ResourceId());
};

}
