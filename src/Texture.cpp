#include "Texture.h"

#include "TextureLoader.h"

namespace ks {

Texture* Texture::load(const std::string filepath, bool absolutePath) {
    return TextureLoader::get().load(filepath, absolutePath);
}

}
