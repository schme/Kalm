#include "Rendering.h"

#include "glad/gl.h"

#if 1
#include "glRendering.h"
#endif

namespace ks::render {

void setupFrame(int fbWidth, int fbHeight)
{
	static const math::vec4 clearColor = math::vec4(0.f, 0.f, 0.f, 1.f);

	clearBuffer(clearColor);
	setupViewport(0, 0, fbWidth, fbHeight);
}

void setPolygonMode(PolygonMode mode)
{
	switch(mode)
	{
	case PolygonMode::Fill: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
	case PolygonMode::Line: {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
	}
}

}
