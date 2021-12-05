#include "Camera.h"

namespace ks {

void updateFreeCameraLook(Camera &camera, math::vec2 offset)
{
	float sensitivity = 0.1f;
	offset *= sensitivity;

	camera.yaw += offset.x;
	camera.pitch -= offset.y;

	if (camera.pitch > 89.f)
		camera.pitch = 89.f;
	if (camera.pitch < -89.f)
		camera.pitch = -89.f;
}

void updateCameraFront(Camera &camera)
{
	/// Setup so that yaw=0, pitch=0, roll=0 is (0,0,-1)
	math::vec3 front;
	front.x = sin(math::radians(camera.yaw)) * cos(math::radians(camera.pitch));
	front.y = sin(math::radians(camera.pitch));
	front.z = -cos(math::radians(camera.yaw)) * cos(math::radians(camera.pitch));

	camera.front = math::normalize(front);

#if 0
	math::vec3 up;
	up.x = cos(math::radians(camera.pitch)) * cos(math::radians(camera.roll));
	up.y = sin(math::radians(camera.roll));
	up.z = sin(math::radians(camera.pitch)) * cos(math::radians(camera.roll));

	camera.up = math::normalize(up);
#endif
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
