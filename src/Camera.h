#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Ray.h"

class Camera
{
public:
	virtual Ray generateRay(const glm::vec2& point) = 0;
	virtual float getTMin() const = 0;
	virtual ~Camera() {}
protected:
	glm::vec3 center;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 horizontal;
};

class PerspectiveCamera : public Camera
{
	float fovx2, fovy2;
public:
	PerspectiveCamera(const glm::vec3& _c, const glm::vec3& _d, const glm::vec3& _u, float fovy, float ar)
	{
		center = _c;
		direction = glm::normalize(_d);
		horizontal = glm::normalize(glm::cross(direction, _u));
		up = glm::normalize(glm::cross(horizontal, direction));
		fovy2 = fovy / 2.0f;
		fovx2 = fovy2 * ar;
	}

	virtual float getTMin() const { return 0.0f; }

	virtual Ray generateRay(const glm::vec2& point)
	{
		float alpha = tanf(fovx2) * point.x;
		float beta = tanf(fovy2) * point.y;
		glm::vec3 rayDirection = (horizontal * alpha) + (up * beta) + direction;
		return Ray(center, rayDirection);
	}
};