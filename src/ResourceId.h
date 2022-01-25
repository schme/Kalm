#pragma once

#include <string>

namespace ks
{

typedef std::string ResourceId;

static inline bool isValid(const ResourceId &id) {
	return !id.empty();
}

static const char *toStr(const ResourceId &id) {
	return id.c_str();
}

}
