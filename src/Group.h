#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"

class Group : public Object3D
{
	int numObjects{ 0 };
	std::vector<Object3D*> objects;
public:
	Group() = delete;
	Group(int nobjs) { numObjects = nobjs; }

	void addObject(int index, Object3D* obj)
	{
		// right now, we don't use index anywhere
		objects.push_back(obj);
		numObjects++;
	}

	int getGroupSize() { return numObjects; }

	virtual bool intersect(const Ray& ray, Hit& hit, float tmin)
	{
		bool isIntersected = false;
		for (auto objPtr : objects)
		{
			isIntersected = isIntersected || objPtr->intersect(ray, hit, tmin);
		}
		return isIntersected;
	}
};