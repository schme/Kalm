#include "Camera.h"

namespace ks {

void updateCameraLook(Camera &camera, math::vec2 offset)
{
	float sensitivity = 0.1f;
	offset *= sensitivity;

	camera.yaw += offset.x;
	camera.pitch -= offset.y;

	if (camera.pitch > 89.f)
		camera.pitch = 89.f;
	if (camera.pitch < -89.f)
		camera.pitch = -89.f;

	math::vec3 front;
	front.x = cos(math::radians(camera.yaw)) * cos(math::radians(camera.pitch));
	front.y = sin(math::radians(camera.pitch));
	front.z = sin(math::radians(camera.yaw)) * cos(math::radians(camera.pitch));

	camera.front = math::normalize(front);
}

void updateCameraPos(Camera &camera, float forward, float right, float up, float magnitude)
{
	math::vec3 cameraVec = {};
	if (!math::isAboutZero(forward)) {
		cameraVec += camera.front * forward;
	}
	if (!math::isAboutZero(right)) {
		cameraVec += math::normalize(math::cross(camera.front, camera.up) * right);
	}
	if (!math::isAboutZero(up)) {
		cameraVec += camera.up * up;
	}

	if (!math::isAboutZero(cameraVec)) {
		cameraVec = math::normalize(cameraVec);
	}

	camera.position += cameraVec * magnitude;
}

}
