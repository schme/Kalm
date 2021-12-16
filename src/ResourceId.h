#pragma once

#include <string>

namespace ks
{

typedef std::string ResourceId;

static inline bool isValid(ResourceId &id) {
	return !id.empty();
}

}
