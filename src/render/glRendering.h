#pragma once

#include "include/maths.h"
#include "gl_utils.h"

namespace ks::render
{

void clearColorBuffer(math::vec4 clearColor);
void clearColorAndDepthBuffer(math::vec4 clearColor);
void setViewport(int x, int y, int width, int height);


}
