#include "Image.h"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>

#define stringify(thing) #thing

#define PPM_BLACK_PIXEL "0 0 0 "
#define PPM_GREEN_PIXEL "107 142 35 "
#define PPM_BROWN_PIXEL "135 74 43 "

enum {
    pxl_black = 0,
    pxl_green,
    pxl_brown
};

Color enumToColor(uint8_t e)
{
    switch (e) {
        default:        return {0,0,0};
        case pxl_green: return {107, 142, 35};
        case pxl_brown: return {135, 74, 43};
    }
}

uint8_t colorToEnum(Color c)
{
    if (c.red == 0 && c.green == 0 && c.blue == 0)
        return pxl_black;
    else if (c.red == 107 && c.green == 142 && c.blue == 35)
        return pxl_green;
    else if (c.red == 135 && c.green == 74 && c.blue == 43)
        return pxl_brown;

    return pxl_black;
}

enum {
    alg_linear,
    alg_spiral
} algorithm = alg_linear;

static int searchForSimilar(Image& i, int x, int y, uint8_t color)
{
    int result = 0;

    // top
    if (colorToEnum(i.getPixel(x - 1, y - 1)) == color) {
        result++;
    }
    if (colorToEnum(i.getPixel(x, y - 1)) == color) {
        result++;
    }
    if (colorToEnum(i.getPixel(x + 1, y - 1)) == color) {
        result++;
    }

    // left and right
    if (colorToEnum(i.getPixel(x - 1, y)) == color) {
        result++;
    }
    if (colorToEnum(i.getPixel(x + 1, y)) == color) {
        result++;
    }

    // bottom
    if (colorToEnum(i.getPixel(x - 1, y + 1)) == color) {
        result++;
    }
    if (colorToEnum(i.getPixel(x, y + 1)) == color) {
        result++;
    }
    if (colorToEnum(i.getPixel(x - 1, y + 1)) == color) {
        result++;
    }

    return result;
}

struct SurroundCount
{
    int black;
    int green;
    int brown;
};

static SurroundCount getSurroundCounts(Image& i, int x, int y)
{
    SurroundCount ret;
    ret.black = searchForSimilar(i, x, y, pxl_black);
    ret.green = searchForSimilar(i, x, y, pxl_green);
    ret.brown = searchForSimilar(i, x, y, pxl_brown);
    return ret;
}

static uint8_t sampleFanbase(Image& i, int x, int y, uint8_t color)
{
    uint8_t newColor = pxl_black;
    SurroundCount surroundings = getSurroundCounts(i, x, y);

    switch (color) {
        case pxl_black:
            if (surroundings.black >= 4) {
                newColor = pxl_black;
                break;
            } else {
                if (surroundings.green == surroundings.brown) {
                    newColor = rand() % 3;
                } else if (surroundings.green > surroundings.brown) {
                    newColor = pxl_green;
                } else {
                    newColor = pxl_brown;
                }
            }
            break;

        case pxl_green:
            if (surroundings.green >= 4) {
                newColor = pxl_green;
                break;
            } else {
                if (surroundings.black == surroundings.brown) {
                    newColor = rand() % 3;
                } else if (surroundings.black > surroundings.brown) {
                    newColor = pxl_black;
                } else {
                    newColor = pxl_brown;
                }
            }
            break;

        case pxl_brown:
            if (surroundings.brown >= 4) {
                newColor = pxl_brown;
                break;
            } else {
                if (surroundings.green == surroundings.black) {
                    newColor = rand() % 3;
                } else if (surroundings.green > surroundings.black) {
                    newColor = pxl_green;
                } else {
                    newColor = pxl_black;
                }
            }
            break;

        default:
            break;
    }

    return newColor;
}

int main(int argc, char* argv[])
{
    int width = 20, height = 20;

    if (argc == 3) {
        std::cout << "argc - " << argc << "\n";

        for (int i = 1; i < argc; i++) {
            std::cout << "argv[" << i << "] - " << argv[i] << "\n";
        }

        sscanf(argv[1], "%d", &width);
        sscanf(argv[2], "%d", &height);

        if (width < 0) width *= -1;
        if (height < 0) height *= -1;
    }

    // init data
    srand(time(NULL));

    Image img{width, height};

    // init data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int randColor = rand() % 3;
            img.setPixel(x, y, enumToColor(randColor));
        }
    }

    ppmPrinter(img, "initial_colors.ppm");

    // do the thing
    int iterations = 0;
    do {
        Image temp{width, height};

        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                uint8_t newColor = sampleFanbase(img, x, y, colorToEnum(img.getPixel(x,y)));
                temp.setPixel(x, y, enumToColor(newColor));
            }
        }

        std::string filename;
        if (algorithm == alg_linear) {
            filename += stringify(alg_linear);
        } else if (algorithm == alg_spiral) {
            filename += stringify(alg_spiral);
        }

        filename += "_result_";

        if (iterations > 9) {
            filename += std::to_string(iterations);
        } else {
            filename += "0";
            filename += std::to_string(iterations);
        }

        filename += "_itr.ppm";

        ppmPrinter(img, filename);

        for (int y = height - 1; y > 1; y--) {
            for (int x = width - 1; x > 1; x--) {
                img.setPixel(x, y, temp.getPixel(x,y));
            }
        }

        iterations++;
    } while (iterations <= 25);

    return 0;
}
