#pragma once

namespace ks
{

struct Player {
	int init(int width, int height);
	int play(bool *shouldQuit);
};

}
