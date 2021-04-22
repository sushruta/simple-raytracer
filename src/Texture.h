#pragma once

#include <glm/glm.hpp>
#include "bitmap_image.h"

class Texture
{
	bitmap_image* bimg{nullptr};
	int width{0};
	int height{0};
public:
    Texture() {}
    ~Texture() { delete bimg;  }

    void operator()(int x, int y, unsigned char* color)
    {
        x = clamp(x, 0, width - 1);
        y = clamp(y, 0, height - 1);
        bimg->get_pixel(x, y, color[0], color[1], color[2]);
    }

    bool Texture::valid()
    {
        return (bimg != nullptr);
    }
    
    glm::vec3 operator()(float x, float y)
    {
        glm::vec3 color;
        int ix, iy;
        x = x * width;
        y = (1 - y) * height;
        ix = (int)x;
        iy = (int)y;
        unsigned char pixels[4][3];
        float alpha = x - ix;
        float beta = y - iy;
        operator()(ix, iy, pixels[0]);
        operator()(ix + 1, iy, pixels[1]);
        operator()(ix, iy + 1, pixels[2]);
        operator()(ix + 1, iy + 1, pixels[3]);
        for (int ii = 0; ii < 3; ii++) {
            color[ii] = (1 - alpha) * (1 - beta) * pixels[0][ii]
                + alpha * (1 - beta) * pixels[1][ii]
                + (1 - alpha) * beta * pixels[2][ii]
                + alpha * beta * pixels[3][ii];
        }
        return color / 255.0f;
    }

    void Texture::load(const char* filename)
    {
        bimg = new bitmap_image(filename);
        height = bimg->height();
        width = bimg->width();
    }
};

