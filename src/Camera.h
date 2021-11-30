#pragma once

#include "include/maths.h"

namespace ks {

struct CameraLens {
	enum class Type {
		Perspective,
		Ortho,
	};
    float fov = 90.f;
    float aspect = 1440.f/900.f;
    float near = 0.1f;
    float far = 100.f;
	Type type = Type::Perspective;
};

struct Camera {
    math::vec3 target = math::vec3(0.f);
    math::vec3 position = math::vec3(0.f, 0.f, 5.f);
    math::vec3 front = math::vec3(0.f, 0.f, -1.f);
    math::vec3 right = math::vec3(-1.f, 0.f, 0.f);
    math::vec3 up = math::vec3(0.f, 1.f, 0.f);

	float yaw = atan2(front.x, front.z);
	float pitch = asin(-front.y);
	float roll = 0;

    CameraLens lens;
};

void updateCameraLook(Camera &camera, math::vec2 offset);

void updateCameraPos(Camera &camera, float forward, float right, float up, float magnitude);

}
