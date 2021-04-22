#pragma once

#include <glm/glm.hpp>

#include "Object3D.h"

///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions

class Plane : public Object3D
{
public:
	Plane() {}
	Plane(const glm::vec3& normal, float d, Material* m) :Object3D(m) {
	}
	~Plane() {}
	virtual bool intersect(const Ray& r, Hit& h, float tmin) {
		return false;
	}

protected:
};