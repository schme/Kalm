#include "Texture.h"

#include "TextureLoader.h"

namespace ks {

Texture* Texture::load(const std::string filepath) {
    return TextureLoader::load(filepath);
}

}
