#pragma once

#include <glm/glm.hpp>

using namespace glm;

class Ray
{
	vec3 origin;
	vec3 direction;
public:
	Ray() = delete;
	Ray(const vec3& origin, const vec3& direction)
	{
		this->origin = origin;
		this->direction = direction;
	}

	Ray(const Ray& ray)
	{
		this->origin = ray.origin;
		this->direction = ray.direction;
	}

	const vec3& getOrigin() const
	{
		return origin;
	}

	const vec3& getDirection() const
	{
		return direction;
	}

	vec3 pointAtParameter(float t) const
	{
		return origin + direction * t;
	}
};