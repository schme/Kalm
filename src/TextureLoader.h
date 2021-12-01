#pragma once

#include "ResourceId.h"
#include "Texture.h"

namespace ks {

struct TextureLoader {

    void init(const std::string& projectRoot) {
        this->projectRoot = projectRoot;
    }

	/// Checks if the texture exists first and returns the already loaded one if so
    Texture* load(const std::string &filepath, bool absolutePath = false);

	Texture* find(const ResourceId &id);

    static TextureLoader& get() {
        static TextureLoader loader;
        return loader;
    }

    std::string projectRoot = "";
};

}
