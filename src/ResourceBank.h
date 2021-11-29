#pragma once

#include <string>
#include <unordered_map>

#include "include/common.h"

namespace ks {

using ResourceId = std::string;

template <typename T>
struct ResourceBank {

    T* add(const std::string &filepath, T&& res) {
        T* foundRes = find(ResourceId(filepath));
        if (foundRes) {
            return foundRes;
        }
        auto it = storage.emplace(ResourceId(filepath), res);
        if (!it.second) return nullptr;
        return it.first->second;
    }

    T* find(const ResourceId &id) {
        auto it = storage.find(id);
        if (it != storage.end()) {
            return &it->second;
        }
        return nullptr;
    }

    static ResourceBank<T>& get() {
        static ResourceBank<T> bank;
        return bank;
    }

    std::unordered_map<ResourceId, T> storage;
};

}
