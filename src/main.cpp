#include <iostream>
#include <string>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"

#include "bitmap_image.h"

int main(int argc, char** argv)
{
    // Fill in your implementation here.

    std::string sceneFilename;
    int width, height;
    std::string outputFilename;
    std::string depthFilename;
    int minDepth, maxDepth;

    // This loop loops over each of the input arguments.
    // argNum is initialized to 1 because the first
    // "argument" provided to the program is actually the
    // name of the executable (in our case, "a4").

    // VERY VERY UGLY string mangling
    int argNum = 1;
    while (argNum < argc)
    {
        if ((std::string(argv[argNum]) == "-size") && argc > argNum + 2)
        {
            std::cout << argv[argNum] << ":  "<< "came for size" << std::endl;
            width = std::stoi(std::string(argv[argNum + 1]));
            height = std::stoi(std::string(argv[argNum + 2]));
            std::cout << width << " and " << height << std::endl;
            argNum += 3;
            continue;
        }
        if ((std::string(argv[argNum]) == "-input") && argc > argNum + 1)
        {
            std::cout << argv[argNum] << ":  " << "came for input" << std::endl;
            sceneFilename = std::string(argv[argNum + 1]);
            std::cout << sceneFilename << std::endl;
            argNum += 2;
            continue;
        }
        if ((std::string(argv[argNum]) == "-output") && argc > argNum + 1)
        {
            std::cout << argv[argNum] << ":  " << "came for output" << std::endl;
            outputFilename = std::string(argv[argNum + 1]);
            std::cout << outputFilename << std::endl;
            argNum += 2;
            continue;
        }
        std::cout << argNum << " and " << argc << " and " << argv[argNum] << std::endl;
        if ((std::string(argv[argNum]) == "-depth") && argc > argNum + 3)
        {
            std::cout << argv[argNum] << ":  " << "came for min and max depth" << std::endl;
            minDepth = std::stoi(std::string(argv[argNum + 1]));
            maxDepth = std::stoi(std::string(argv[argNum + 2]));
            depthFilename = std::string(argv[argNum + 3]);
            std::cout << depthFilename << std::endl;
            std::cout << minDepth << " and " << maxDepth << std::endl;
            argNum += 4;
            continue;
        }
        std::cout << "hmm... should not come here" << std::endl;
        argNum += 1;
    }

    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    SceneParser sp = SceneParser(sceneFilename);
    Image image(width, height);

    glm::vec3 fgColor(0.8, 0.2, 0.0);
    glm::vec3 bgColor(0.0f, 0.2f, 0.4f);

    image.SetAllPixels(bgColor);

    for (int r = 0; r < width; r++)
    {
        for (int c = 0; c < height; c++)
        {
            // first let us generate the ray
            float i = (height / 2.0 - c) / (height / 2.0);
            float j = (r - width / 2.0) / (width / 2.0);
            Ray ray = (sp.getCamera())->generateRay(glm::vec2(j, i));

            // now let us get the intersection for this ray
            Hit hit;
            bool intersectionFound = sp.getGroup()->intersect(ray, hit, sp.getCamera()->getTMin());
            if (intersectionFound)
            {
                image.SetPixel(r, c, fgColor);
            }
        }
    }
    image.SaveImage(outputFilename);

    return 0;
}