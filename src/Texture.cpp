#include "Texture.h"

#include "TextureBank.h"

namespace ks {

Texture* Texture::load(const std::string filepath, bool absolutePath) {
    return TextureBank::get().load(filepath, absolutePath);
}

}
