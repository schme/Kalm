#pragma once

#include "Texture.h"

namespace ks {

struct TextureLoader {

    void init(const std::string& projectRoot) {
        this->projectRoot = projectRoot;
    }

    static Texture* load(const std::string &filepath);

    static TextureLoader& get() {
        static TextureLoader loader;
        return loader;
    }

    std::string projectRoot;
};

}
