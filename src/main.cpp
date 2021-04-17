#include <iostream>
#include "Ray.h"
#include <glm/glm.hpp>

int main(int argc, char **argv)
{
    std::cout << "Hello cmake world" << std::endl;

    Ray r = Ray(vec3(0, 0, 0), vec3(0, 0, -1));
    std::cout << r.pointAtParameter(2.0).z << std::endl;
}