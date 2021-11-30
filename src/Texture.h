#pragma once

#include <cstddef>
#include <string>

namespace ks {

struct Texture {

    static Texture* load(const std::string filepath);

    void* data = nullptr;
	int channels = 0;
	size_t width = 0;
	size_t height = 0;
};

}
