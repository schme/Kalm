#pragma once

#include "ResourceStorage.h"
#include "StaticSingleton.h"
#include "ResourceId.h"

namespace ks {

template <typename T, typename U>
struct ResourceBank : public StaticSingleton<U>{
    virtual void init() {}

	/// Should check if the resource exists first and return the already loaded one if so
    virtual T* load(const std::string &filepath, bool absolutePath = false) {
		ks_unused(filepath);
		ks_unused(absolutePath);
		return nullptr;
	}

	/// Prefer this whenever possible
	constexpr T* find(const ResourceId &id) {
		return ResourceStorage<T>::get().find(id);
	}

	bool isLoaded(const ResourceId &id) {
		return find(id) != nullptr;
	}
};

}
