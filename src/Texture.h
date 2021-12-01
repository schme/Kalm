#pragma once

#include <cstddef>
#include <string>

#include "include/common.h"

namespace ks {

using TextureId = u32;

struct Texture {

    static Texture* load(const std::string filepath);

	TextureId id = 0;
    void* data = nullptr;
	int channels = 0;
	size_t width = 0;
	size_t height = 0;
};

}
