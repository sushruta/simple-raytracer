#pragma once

#include "Object3D.h"
#include "Hit.h"

class Sphere : public Object3D
{
	glm::vec3 center;
	float radius;
	Material* material;
	glm::mat4 transformation{glm::mat4(1.0f)};
public:
	Sphere() = delete;
	Sphere(const glm::vec3& _c, float _r, Material* _m) : Object3D(_m)
	{
		center = _c;
		radius = _r;
		material = _m;

		float recr = 1.0f / _r;

		// create a transformation to bring the sphere to origin
		// and make it unit radius
		transformation = glm::translate(transformation, _c * -1.0f);
		transformation = glm::scale(transformation, glm::vec3(recr, recr, recr));
	}

	virtual bool intersect(const Ray& ray, Hit& hit, float tmin) {
		glm::vec3 oc = ray.getOrigin() - center;

		float oc2 = glm::dot(oc, oc);
		float d2 = glm::dot(ray.getDirection(), ray.getDirection());
		float oc_dot_d = glm::dot(ray.getDirection(), oc);

		float disc = (oc_dot_d * oc_dot_d) - d2 * (oc2 - radius * radius);
		if (disc < 0) {
			return false;
		}
		std::cout << "discriminant value: " << disc << std::endl;

		float disc_root = sqrtf(disc);
		float t1 = (-oc_dot_d - disc_root) / d2;
		float t2 = (-oc_dot_d + disc_root) / d2;

		tmin = fmin(tmin, fmin(t1, t2));
		if (tmin < hit.getT())
		{
			glm::vec3 pt = ray.pointAtParameter(tmin);
			glm::vec3 normal = pt - center;
			hit = Hit(tmin, material, normal);
			// todo: add texture if it has it
			return true;
		}

		return false;
	}
};