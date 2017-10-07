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

Color first{0, 0, 0, 255};
Color second{107, 142, 35, 255};
Color third{135, 74, 43, 255};

Color enumToColor(uint8_t e) {
    switch (e) {
        default:
            return first;
        case pxl_green:
            return second;
        case pxl_brown:
            return third;
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

    uint32_t left = x - 1,
             right = x + 1,
             top = y - 1,
             bottom = y + 1;

    if (x == 0) left = i.width - 1;
    if (x == i.width) right = 0;
    if (y == 0) top = i.height - 1;
    if (y == i.height) bottom = 0;

    // top
    if (colorToEnum(i.get_pixel(left, top)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel(x, top)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel(right, top)) == color) {
        result++;
    }

    // left and right
    if (colorToEnum(i.get_pixel(left, y)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel(right, y)) == color) {
        result++;
    }

    // bottom
    if (colorToEnum(i.get_pixel(left, bottom)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel(x, bottom)) == color) {
        result++;
    }
    if (colorToEnum(i.get_pixel(right, bottom)) == color) {
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

void parse(char **args, uint32_t &width, uint32_t &height, uint32_t &twidth, uint32_t &theight) {
    sscanf(args[1], "%d", &width);
    sscanf(args[2], "%d", &height);

    double aspect_ratio = (double)width / (double)height;
    theight = (double)height / 2.5;
    twidth = theight * aspect_ratio;
}

int main(int argc, char *argv[]) {
    uint32_t width = 800, height = 600;
    uint32_t tex_width = 300, tex_height = 225;

    if (argc == 3) {
        printf("argc: %d\n", argc);
        parse(argv, width, height, tex_width, tex_height);
    }

    printf("Width: %d  Height: %d  Texture Width: %d  Texture Height: %d\n",
        width, height, tex_width, tex_height);

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

    Texture img(tex_width, tex_height);

    // init data
    for (uint32_t y = 0; y < tex_height; y++) {
        for (uint32_t x = 0; x < tex_width; x++) {
            uint32_t randColor = rand() % 3;
            img.set_pixel(x, y, enumToColor(randColor));
        }
    }

    SDL_Texture *drawTex = SDL_CreateTexture(r,
                                             SDL_PIXELFORMAT_RGBA32,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             tex_width, tex_height);

    bool regen = false;
    bool colorize = false;
    bool quit = false;
    do {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_r) {
                    regen = true;
                } else if (e.key.keysym.sym == SDLK_c) {
                    //colorize = true;
                } else {
                    quit = true;
                }
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                    quit = true;
                }
            }
        }

        if (colorize) {
            colorize = false;
            first.red = rand() % 255;
            first.green = rand() % 255;
            first.blue = rand() % 255;

            second.red = rand() % 255;
            second.green = rand() % 255;
            second.blue = rand() % 255;

            third.red = rand() % 255;
            third.green = rand() % 255;
            third.blue = rand() % 255;
        }

        if (regen) {
            regen = false;

            for (uint32_t y = 0; y < tex_height; y++) {
                for (uint32_t x = 0; x < tex_width; x++) {
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
        Texture temp(tex_width, tex_height);

        for (uint32_t y = 0; y < tex_height; y++) {
            for (uint32_t x = 0; x < tex_width; x++) {
                uint8_t newColor =
                    sampleFanbase(img, x, y, colorToEnum(img.get_pixel(x, y)));
                temp.set_pixel(x, y, enumToColor(newColor));
            }
        }

        for (uint32_t y = 0; y < tex_height; y++) {
            for (uint32_t x = 0; x < tex_width; x++) {
                img.set_pixel(x, y, temp.get_pixel(x,y));
            }
        }
//*/
        // update texture and draw on screen
        SDL_UpdateTexture(drawTex, nullptr, reinterpret_cast<const void *>(img.data()), tex_width*4);
        SDL_RenderCopy(r, drawTex, nullptr, nullptr);
        SDL_RenderPresent(r);
    } while(!quit);

    SDL_DestroyTexture(drawTex);
    SDL_DestroyRenderer(r);
    SDL_Quit();
    return 0;
}
