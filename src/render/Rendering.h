#pragma once

#include "include/maths.h"

namespace ks::render {

enum class PolygonMode {
	Fill,
	Line,
};

void setupFrame(int fbWidth, int fbHeight);
void setPolygonMode(PolygonMode mode);

}
