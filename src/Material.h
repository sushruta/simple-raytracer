#pragma once

#include <optional>
#include <glm/glm.hpp>

#include "Ray.h"
#include "Hit.h"
#include "Texture.h"


class Material
{
public:
	Material(const glm::vec3& _d, const glm::vec3& _s = glm::vec3(0.0), float _sh = 0) : diffuseColor(_d), specularColor(_s), shininess(_sh) {}

	void loadTexture(const char* filename)
	{
		t.load(filename);
	}

	glm::vec3 shade(const Ray& ray, const Hit& hit, const glm::vec3& directionToLight, const glm::vec3& lightColor)
	{
		return glm::vec3(1.0);
	}

protected:
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	Texture t;
};

