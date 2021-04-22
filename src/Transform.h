#pragma once

#include <glm/glm.hpp>

#include "Object3D.h"

class Transform : public Object3D
{
protected:
	Object3D* obj;
	glm::mat4 transMat;
public:
	Transform() {};
	Transform(const glm::mat4& m, Object3D *_obj) : obj(_obj)
	{
		obj = _obj;
		transMat = m;
	}

	virtual bool intersect(const Ray& ray, Hit& hit, float tmin)
	{
		glm::vec4 dir4 = glm::inverse(transMat) * glm::vec4(ray.getDirection(), 1.0f);
		glm::vec3 dir3 = glm::vec3(dir4) / dir4.w;

		glm::vec4 orig4 = glm::inverse(transMat) * glm::vec4(ray.getOrigin(), 1.0f);
		glm::vec3 orig3 = glm::vec3(orig4) / orig4.w;

		return obj->intersect(Ray(orig3, dir3), hit, tmin);
	}
};