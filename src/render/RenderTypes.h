#pragma once

#include "include/common.h"

namespace ks {

struct FramebufferInfo {
	u32 fboId = 0;
	u32 colorAttachmentId = 0;
	u32 stencilDepthBufferId = 0;
};

}
