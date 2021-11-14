#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <memory>

#include "include/maths.h"
#include "gui/TimelineGui.h"

namespace ks {

struct Gui {
	bool init(GLFWwindow *window, int currentFrame, int maxFrames, float playbackRate);
	void run();
	void render();
	void terminate();

	static Gui &get() {
		static Gui gui;
		return gui;
	}

	math::vec3 clear_color = math::vec3(0.05f, 0.05f, 0.05f);
	GLFWwindow *window = nullptr;

	std::unique_ptr<TimelineGui> timelineGui;

	bool optShowDemoWindow = false;
	bool optShowTimeline = true;
};

}
