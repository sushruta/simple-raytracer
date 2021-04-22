#pragma once

#include <optional>

#include "Object3D.h"

class Triangle : public Object3D
{
public:
	bool hasTex{ false };
	glm::vec3 normals[3];
	glm::vec2 texCoords[3];

	Triangle() = delete;
	Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, Material* m) : Object3D(material) {
		hasTex = false;
	}

	virtual bool intersect(const Ray& ray, Hit& hit, float tmin)
	{
		return false;
	}
};