//#include "Image.h"

//#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
//#include <fstream>
#include <iostream>
//#include <string>

#include "texture.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#define stringify(thing) #thing

#define PPM_BLACK_PIXEL "0 0 0 "
#define PPM_GREEN_PIXEL "107 142 35 "
#define PPM_BROWN_PIXEL "135 74 43 "

enum { pxl_black = 0, pxl_green, pxl_brown };

Color enumToColor(uint8_t e) {
    switch (e) {
        default:
            return {0, 0, 0, 255};
        case pxl_green:
            return {107, 142, 35, 255};
        case pxl_brown:
            return {135, 74, 43, 255};
    }
}

uint8_t colorToEnum(Color c) {
    if (c.red == 0 && c.green == 0 && c.blue == 0)
        return pxl_black;
    else if (c.red == 107 && c.green == 142 && c.blue == 35)
        return pxl_green;
    else if (c.red == 135 && c.green == 74 && c.blue == 43)
        return pxl_brown;

    return pxl_black;
}

enum { alg_linear, alg_spiral } algorithm = alg_linear;

static uint32_t searchForSimilar(Texture &i, uint32_t x, uint32_t y, uint8_t color) {
    uint32_t result = 0;

    // top
    if (colorToEnum(i.get_pixel((x - 1) % i.width, (y - 1) % i.height)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel(x, (y - 1) % i.height)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel((x + 1) % i.width, (y - 1) % i.height)) == color) {
        result++;
    }

    // left and right
    if (colorToEnum(i.get_pixel((x - 1) % i.width, y)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel((x + 1) % i.width, y)) == color) {
        result++;
    }

    // bottom
    if (colorToEnum(i.get_pixel((x - 1) % i.width, (y + 1) % i.height)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel(x, (y + 1) % i.width)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel((x - 1) % i.width, (y + 1) % i.height)) == color) {
        result++;
    }

    return result;
}

struct SurroundCount {
    uint32_t black;
    uint32_t green;
    uint32_t brown;
};

static SurroundCount getSurroundCounts(Texture &i, uint32_t x, uint32_t y) {
    SurroundCount ret;
    ret.black = searchForSimilar(i, x, y, pxl_black);
    ret.green = searchForSimilar(i, x, y, pxl_green);
    ret.brown = searchForSimilar(i, x, y, pxl_brown);
    return ret;
}

static uint8_t sampleFanbase(Texture &i, uint32_t x, uint32_t y, uint8_t color) {
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

int main(int argc, char *argv[]) {
    uint32_t width = 512, height = 512;

    if (argc == 3) {
        std::cout << "argc - " << argc << "\n";

        for (int i = 1; i < argc; i++) {
            std::cout << "argv[" << i << "] - " << argv[i] << "\n";
        }

        sscanf(argv[1], "%d", &width);
        sscanf(argv[2], "%d", &height);
    }

    // init data
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to init SDL2.\n";
        return -1;
    }

    SDL_Window *w;
    SDL_Renderer *r;
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &w, &r)) {
        std::cout << "Failed to create window and renderer.\n";
        return -1;
    }

    Texture img(width, height);

    // init data
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint32_t randColor = rand() % 3;
            img.set_pixel(x, y, enumToColor(randColor));
        }
    }

    SDL_Texture *drawTex = SDL_CreateTexture(r,
                                             SDL_PIXELFORMAT_RGBA32,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width, height);

    bool randomize = false;
    bool quit = false;
    do {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_r) {
                    randomize = true;
                } else {
                    quit = true;
                }
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                    quit = true;
                }
            }
        }

        if (randomize) {
            randomize = false;

            for (uint32_t y = 0; y < height; y++) {
                for (uint32_t x = 0; x < width; x++) {
                    uint32_t randColor = rand() % 3;
                    img.set_pixel(x, y, enumToColor(randColor));
                }
            }
        }

        // clear screen
        SDL_SetRenderDrawColor(r, 0, 0, 0, 0xFF);
        SDL_RenderClear(r);

//*
        // update pattern
        Texture temp(width, height);

        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                uint8_t newColor =
                    sampleFanbase(img, x, y, colorToEnum(img.get_pixel(x, y)));
                temp.set_pixel(x, y, enumToColor(newColor));
            }
        }

        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                img.set_pixel(x, y, temp.get_pixel(x,y));
            }
        }
//*/
        // update texture and draw on screen
        SDL_UpdateTexture(drawTex, nullptr, reinterpret_cast<const void *>(img.data()), width*4);
        SDL_RenderCopy(r, drawTex, nullptr, nullptr);
        SDL_RenderPresent(r);
    } while(!quit);
    SDL_DestroyRenderer(r);
    SDL_Quit();

    /*
    ppmPrinter(img, "initial_colors.ppm");

    // do the thing
    int iterations = 0;
    do {
        Image temp{width, height};

        for (int y = 0; y < height - 1; y++) {
            for (int x = 0; x < width - 1; x++) {
                uint8_t newColor =
                    sampleFanbase(img, x, y, colorToEnum(img.get_pixel(x, y)));
                temp.set_pixel(x, y, enumToColor(newColor));
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

        for (int y = height - 1; y >= 0; y--) {
            for (int x = width - 1; x >= 0; x--) {
                img.set_pixel(x, y, temp.get_pixel(x, y));
            }
        }

        iterations++;
    } while (iterations <= 50);
    */

    return 0;
}
