#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>


class Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
public:
	Ray() = delete;
	Ray(const glm::vec3& origin, const glm::vec3& direction)
	{
		this->origin = origin;
		this->direction = direction;
	}

	Ray(const Ray& ray)
	{
		this->origin = ray.origin;
		this->direction = ray.direction;
	}

	const glm::vec3& getOrigin() const
	{
		return origin;
	}

	const glm::vec3& getDirection() const
	{
		return direction;
	}

	// TODO: need to rewrite this function
	glm::vec3 pointAtParameter(float t) const
	{
		return origin + normalize(direction) * t;
	}
};

inline std::ostream& operator << (std::ostream& os, const Ray& r)
{
	os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
	return os;
}