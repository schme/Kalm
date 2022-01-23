#include "glRendering.h"

#include "glad/gl.h"

namespace ks::render
{

void clearColorBuffer(math::vec4 clearColor)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}
void clearColorAndDepthBuffer(math::vec4 clearColor)
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

}
