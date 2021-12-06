#pragma once

#include "ResourceId.h"
#include "Scene.h"
#include "ResourceBank.h"

#include <string>

namespace ks {

struct SceneBank : public ResourceBank<Scene, SceneBank> {
	
    Scene* load(const std::string &filepath, bool absolutePath = false) override;
};

}
