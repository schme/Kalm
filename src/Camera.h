#pragma once

#include "include/maths.h"

namespace ks {

struct CameraLens {
    float fov = 90.f;
    float aspect = 1440.f/900.f;
    float near = 1.f;
    float far = -100.f;
};

struct Camera {
    math::vec3 target = math::vec3(0.f);
    math::vec3 position = math::vec3(0.f, 0.f, 5.f);
    math::vec3 front = math::vec3(0.f, 0.f, -1.f);
    math::vec3 right = math::vec3(-1.f, 0.f, 0.f);
    math::vec3 up = math::vec3(0.f, 1.f, 0.f);

    CameraLens lens;
};

}
