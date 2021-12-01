#pragma once

#include "include/maths.h"
#include "gl_utils.h"

namespace ks::render
{

void clearBuffer(math::vec4 clearColor);
void setupViewport(int x, int y, int width, int height);


}
