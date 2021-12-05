#pragma once

#include "ResourceBank.h"
#include "ResourceId.h"
#include "Texture.h"

namespace ks {

struct TextureBank : public ResourceBank<Texture, TextureBank> {

    Texture* load(const std::string &filepath, bool absolutePath = false) override;
};

}
