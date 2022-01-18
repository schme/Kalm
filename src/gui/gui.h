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

	GLFWwindow *window = nullptr;
	TimelineGui timelineGui;

	bool optShowDemoWindow = false;
	bool optShowTimeline = true;
	bool optShowCameraWindow = false;
	bool optShowSceneWindow = true;
	bool optShowTextureWindow = true;
	bool optShowShadersWindow = true;
	bool optShowTimelinePreview = true;
	bool optShowMaterialWindow = true;

	static Gui &get() {
		static Gui gui;
		return gui;
	}

};

}
