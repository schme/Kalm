#pragma once

#include "ResourceId.h"

namespace ks {

struct TimelineItem {

	enum class Type : int {
		Scene = 0,
		Post,
		Count
	};

	ResourceId id;
	int frameStart = 0;
	int frameEnd = 120;
	Type type = Type::Count;
};

}
