#pragma once

#include "ResourceId.h"

namespace ks {

struct Material {
	ResourceId shader;
	ResourceId texture0;
	ResourceId texture1;
	ResourceId texture2;
};

}
