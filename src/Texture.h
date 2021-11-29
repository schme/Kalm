#pragma once

#include <cstddef>
#include <string>

namespace ks {

struct Texture {

    static Texture* load(const std::string filepath);

    void *data = nullptr;
    size_t size = 0;
};

}
