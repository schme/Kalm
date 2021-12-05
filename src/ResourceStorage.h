#pragma once

#include <unordered_map>

#include "StaticSingleton.h"
#include "ResourceId.h"
#include "include/common.h"

namespace ks {

template <typename T>
struct ResourceStorage : public StaticSingleton<ResourceStorage<T>>{

    T* add(const std::string &stringId, T&& res) {
        T* foundRes = find(ResourceId(stringId));
        if (foundRes) {
            return foundRes;
        }
        auto it = storage.emplace(ResourceId(stringId), res);
        if (!it.second) return nullptr;
        return &it.first->second;
    }

    T* find(const ResourceId &id) {
        auto it = storage.find(id);
        if (it != storage.end()) {
            return &it->second;
        }
        return nullptr;
    }

    std::unordered_map<ResourceId, T> storage;
};

}
