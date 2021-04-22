#include <glm/glm.hpp>
#include <string>

// Simple image class
class Image
{

public:

    Image(int w, int h)
    {
        width = w;
        height = h;
        data = new glm::vec3[width * height];
    }

    ~Image()
    {
        delete[] data;
    }

    int Width() const
    {
        return width;
    }

    int Height() const
    {
        return height;
    }

    const glm::vec3& GetPixel(int x, int y) const
    {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        return data[y * width + x];
    }

    void SetAllPixels(const glm::vec3& color)
    {
        for (int i = 0; i < width * height; ++i)
        {
            data[i] = color;
        }
    }

    void SetPixel(int x, int y, const glm::vec3& color)
    {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        data[y * width + x] = color;
    }

    static Image* LoadPPM(const char* filename);
    void SavePPM(const char* filename) const;

    static Image* LoadTGA(const std::string& filename);
    void SaveTGA(const std::string& filename) const;
    int SaveBMP(const std::string& filename);
    void SaveImage(const std::string& filename);
    // extension for image comparison
    static Image* compare(Image* img1, Image* img2);

private:

    int width;
    int height;
    glm::vec3 *data;

};