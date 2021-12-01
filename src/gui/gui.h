#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "include/maths.h"
#include "gui/TimelineGui.h"

namespace ks {

struct EditorState;

struct Gui {
	bool init(GLFWwindow *window);
	void run(EditorState &state);
	void render();
	void terminate();

	math::vec3 clear_color = math::vec3(0.05f, 0.05f, 0.05f);
	GLFWwindow *window = nullptr;

	bool optShowDemoWindow = false;
	bool optShowTimeline = true;
	bool optShowCameraWindow = false;
	bool optShowSceneWindow = true;

	static Gui &get() {
		static Gui gui;
		return gui;
	}

};

}
