#pragma once

#include <glm/glm.hpp>

#include "Object3D.h"

class Light
{
public:

    Light()
    {

    }

    virtual ~Light()
    {

    }

    virtual void getIllumination(const glm::vec3& p, glm::vec3& dir, glm::vec3& col, float& distanceToLight) const = 0;

};

class DirectionalLight : public Light
{
public:

    DirectionalLight(const glm::vec3& d, const glm::vec3& c)
    {
        direction = glm::normalize(d);
        color = c;
    }

    ~DirectionalLight()
    {

    }
    ///@param p unsed in this function
    ///@param distanceToLight not well defined because it's not a point light
    virtual void getIllumination(const glm::vec3& p, glm::vec3& dir, glm::vec3& col, float& distanceToLight) const
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = -direction;
        col = color;
    }

private:

    DirectionalLight(); // don't use

    glm::vec3 direction;
    glm::vec3 color;

};

class PointLight : public Light
{
public:

    PointLight(const glm::vec3& p, const glm::vec3& c)
    {
        position = p;
        color = c;
    }

    ~PointLight()
    {

    }

    virtual void getIllumination(const glm::vec3& p, glm::vec3& dir, glm::vec3& col, float& distanceToLight) const
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = (position - p);
        dir = glm::normalize(dir);
        col = color;
    }

private:

    PointLight(); // don't use

    glm::vec3 position;
    glm::vec3 color;

};
