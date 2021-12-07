#pragma once

#include "ResourceBank.h"
#include "ResourceId.h"
#include "Scene.h"

namespace ks {

struct SceneBank : public ResourceBank<Scene, SceneBank> {

    Scene* load(const std::string &filepath, bool absolutePath = false) override;

};

}
