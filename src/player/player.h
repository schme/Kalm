#pragma once

#include <cstdint>

namespace ks
{

struct Player {

	int init(int width, int height);
	int play(uint64_t demoTime, bool *shouldQuit);

	uint64_t prevTickTime = 0;
};

}
