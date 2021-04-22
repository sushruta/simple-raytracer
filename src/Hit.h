#pragma once

#include <limits>
#include <optional>
#include <glm/glm.hpp>

class Material;

class Hit
{
	float t{std::numeric_limits<float>::max()};
	bool hasTex{ false };
	glm::vec2 texCoord;
	Material* material{nullptr};
	glm::vec3 normal;

public:
	Hit() {};

	Hit(float _t, Material *m, const glm::vec3& n) {
		set(_t, m, n);
	}

	Hit(const Hit& hit)
	{
		set(hit.t, hit.material, hit.getNormal());
		if (hit.isTexDefined())
		{
			texCoord = hit.texCoord;
			hasTex = true;
		}
	}

	void set(float _t, Material *m, const glm::vec3& n)
	{
		t = _t;
		material = m;
		normal = n;
	}


	void setTexCoord(const glm::vec2& tcoord)
	{
		texCoord = tcoord;
		hasTex = true;
	}

	float getT() const { return t; }
	Material* getMaterial() const { return material; }
	glm::vec3 getNormal() const { return normal; }
	bool isTexDefined() const { return hasTex; }
	glm::vec2 getTexCoord() const { return texCoord; }
};

inline std::ostream& operator << (std::ostream& os, const Hit& h)
{
	os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
	return os;
}

