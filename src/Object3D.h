#pragma once

#include <optional>

#include "Ray.h"
#include "Hit.h"
#include "Material.h"

class Object3D
{
protected:
	Material* material{nullptr};
public:
	Object3D() {}
	virtual ~Object3D() {}

	Object3D(Material* material) { this->material = material; }

	virtual bool intersect(const Ray& ray, Hit& hit, float tmin) = 0;
};