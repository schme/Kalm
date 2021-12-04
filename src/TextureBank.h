#pragma once

#include "ResourceId.h"
#include "Texture.h"
#include "StaticSingleton.h"

namespace ks {

struct TextureBank : StaticSingleton<TextureBank> {

    void init(const std::string& projectRoot);
	/// Checks if the texture exists first and returns the already loaded one if so
    Texture* load(const std::string &filepath, bool absolutePath = false);
	Texture* find(const ResourceId &id);

    std::string projectRoot = "";
};

}
