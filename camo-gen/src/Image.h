#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cstdint>
#include <string>
#include <fstream>

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Image
{
    int width;
    int height;
    Color** data_buffer;

public:
    Image(int width, int height)
    {
        this->width = width;
        this->height = height;

        data_buffer = new Color*[height];
        for (int i = 0; i < height; i++) {
            data_buffer[i] = new Color[width];
        }
    }
    Image(const Image &) = delete;
    Image& operator=(const Image &) = delete;


    ~Image()
    {
        for (int i = 0; i < height; i++) {
            delete [] data_buffer[i];
        }
        delete [] data_buffer;
    }

    int getWidth()
    {
        return this->width;
    }

    int getHeight()
    {
        return this->height;
    }

    void setPixel(int x, int y, Color c)
    {
        if (x < this->width && y < this->height) {
            data_buffer[y][x] = c;
        }
    }

    Color getPixel(int x, int y)
    {
        if (x < this->width && y < this->height) {
            return data_buffer[y][x];
        } else {
            return {0,0,0};
        }
    }
};

void ppmPrinter(Image& i, std::string filename)
{
    std::ofstream ofs(filename);
    ofs << "P3\n";
    ofs << i.getWidth() << " " << i.getHeight() << "\n";
    ofs << "255\n";

    for (int y = 0; y < i.getWidth(); y++) {
        for (int x = 0; x < i.getHeight(); x++) {
            Color c = i.getPixel(x, y);
            ofs << (int)c.red << " ";
            ofs << (int)c.green << " ";
            ofs << (int)c.blue << " ";
        }
        ofs << std::endl;
    }
    ofs.close();
}

#endif /* _IMAGE_H_ */
