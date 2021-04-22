#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Object3D.h"
#include "Mesh.h"
#include "Group.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Transform.h"

#define MAX_PARSER_TOKEN_LENGTH 100

class SceneParser
{
    SceneParser() = delete;

    void parseFile();
    void parsePerspectiveCamera();
    void parseBackground();
    void parseLights();
    Light* parseDirectionalLight();
    Light* parsePointLight();
    void parseMaterials();
    Material* parseMaterial();

    Object3D* parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
    Group* parseGroup();
    Sphere* parseSphere();
    Plane* parsePlane();
    Triangle* parseTriangle();
    Mesh* parseTriangleMesh();
    Transform* parseTransform();

    int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);
    glm::vec3 readVec3();
    glm::vec2 readVec2();
    float readFloat();
    int readInt();

    FILE* file;
    Camera* camera{nullptr};
    glm::vec3 background_color{ glm::vec3(0.5, 0.5, 0.5) };
    glm::vec3 ambient_light{ glm::vec3(0, 0, 0) };
    int num_lights;
    Light** lights{nullptr};
    int num_materials{0};
    Material** materials{nullptr};
    Material* current_material{nullptr};
    Group* group{nullptr};
public:

    SceneParser(const std::string& filename);
    ~SceneParser();

    Camera* getCamera() const
    {
        return camera;
    }

    glm::vec3 getBackgroundColor() const
    {
        return background_color;
    }

    glm::vec3 getAmbientLight() const
    {
        return ambient_light;
    }

    int getNumLights() const
    {
        return num_lights;
    }

    Light* getLight(int i) const
    {
        assert(i >= 0 && i < num_lights);
        return lights[i];
    }

    int getNumMaterials() const
    {
        return num_materials;
    }

    Material* getMaterial(int i) const
    {
        assert(i >= 0 && i < num_materials);
        return materials[i];
    }

    Group* getGroup() const
    {
        return group;
    }
};
