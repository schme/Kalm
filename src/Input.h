#pragma once

#include "include/maths.h"
#include "include/common.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#

namespace ks {

struct InputState {
	math::vec2 mousePos = {};
	math::vec2 mouseDelta = {};
	math::vec2 leftDragStart = {};
	math::vec2 rightDragStart = {};
	bool lmb = false;
	bool rmb = false;

	bool esc = false;
	bool space = false;
	bool shift = false;

	float forward = 0.0f;
	float right = 0.0f;
};

static InputState inputState[2];
static int currentInputIndex = 0;
static int lastInputIndex = 1;

static InputState bufferState;

static void applyInputBuffer(InputState &buffer, InputState &dest, InputState &last)
{
	// Calculations
	buffer.mouseDelta = last.mousePos - buffer.mousePos;
	if (buffer.lmb && !last.lmb) {
		buffer.leftDragStart = buffer.mousePos;
	}
	if (buffer.rmb && !last.rmb) {
		buffer.rightDragStart = buffer.mousePos;
	}

	memcpy(&dest, &buffer, sizeof(InputState));
}

static inline void swapInputStates()
{
	// Swap state indices
	int tmp = currentInputIndex;
	currentInputIndex = lastInputIndex;
	lastInputIndex = tmp;

	applyInputBuffer(bufferState, inputState[currentInputIndex], inputState[lastInputIndex]);
}

static inline InputState &getCurrentInputState()
{
	return inputState[currentInputIndex];
}
static inline InputState &getLastInputState()
{
	return inputState[lastInputIndex];
}
static inline InputState &getInputBuffer()
{
	return bufferState;
}

static void errorCallback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	InputState &state = getInputBuffer();

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		state.rmb = (action == GLFW_PRESS) ? true : false;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		state.lmb = (action == GLFW_PRESS) ? true : false;
	}
}

static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	ks_unused(window);
	getInputBuffer().mousePos = {xpos, ypos};
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ks_unused(scancode);
	ks_unused(mods);

	int KEY_DOWN = GLFW_PRESS | GLFW_REPEAT;

	InputState &state = getInputBuffer();

	if (key == GLFW_KEY_ESCAPE) {
		state.esc = (action & KEY_DOWN);
	}
	if (key == GLFW_KEY_SPACE) {
		state.space = (action & KEY_DOWN);
	}
	if (key == GLFW_KEY_W) {
		state.forward = (action & KEY_DOWN) ? 1.0f : 0.0f;
	}
	if (key == GLFW_KEY_A) {
		state.right = (action & KEY_DOWN) ? -1.0f : 0.0f;
	}
	if (key == GLFW_KEY_S) {
		state.forward = (action & KEY_DOWN) ? -1.0f : 0.0f;
	}
	if (key == GLFW_KEY_D) {
		state.right = (action & KEY_DOWN) ? 1.0f : 0.0f;
	}

	if (key == GLFW_KEY_LEFT_SHIFT) {
		state.shift = (action & KEY_DOWN) ? true : false;
	}
}
}
