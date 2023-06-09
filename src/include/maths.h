#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
 
namespace ks::math {
	using namespace glm;

	struct AABB {
		vec3 min;
		vec3 max;
	};

	inline bool isAboutZero(const math::vec3 &vec)
	{
		return math::length2(vec) < 0.0001f;
	}
	inline bool isAboutZero(float val)
	{
		return (val * val) < 0.0001f;
	}

}
