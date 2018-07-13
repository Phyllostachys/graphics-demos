#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

//static const double M_PI = 3.1415;

#define RENDER_X 24
#define RENDER_Y 24

struct Pixel {
    uint8_t alpha;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

void random(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t time, struct Pixel *p) {
    p->red   = (uint8_t)rand();
    p->green = (uint8_t)rand();
    p->blue  = (uint8_t)rand();
    p->alpha = 0xFF;
}

void wave(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t time, struct Pixel *p) {
    double u = (double)x / (double)width;
    double v = (double)y / (double)height;
    double t = (double)time / 1000.0;

    p->red   = (uint8_t)(255 * (0.5 + 0.5 * cos(t + u + 0)));
    p->green = (uint8_t)(255 * (0.5 + 0.5 * cos(t + v + 2)));
    p->blue  = (uint8_t)(255 * (0.5 + 0.5 * cos(t + u + 4)));
    p->alpha = 0xFF;
}

void update(struct Pixel *buffer, uint32_t width, uint32_t height, uint32_t time) {
    struct Pixel p;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if ((time % 10000) > 5000)
                wave(x, y, width, height, time, &p);
            else
                random(x, y, width, height, time, &p);
            buffer[width * y + x] = p;
        }
    }
}

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    srand(time(0));

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        puts("Fail init?");
    }

    if (SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer)) {
        puts("Failed to create window and renderer.");
    }
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 0xFF);

    //struct Pixel buffer[RENDER_X * RENDER_Y];
    struct Pixel *buffer = (struct Pixel *)malloc(sizeof(struct Pixel) * RENDER_X * RENDER_Y);
    if (!buffer) {
        puts("Failed to allocate frame buffer!");
    }

    SDL_Texture *texture;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, RENDER_X, RENDER_Y);
    uint32_t start = SDL_GetTicks();
    bool quit = false;

    while (!quit) {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                quit = true;
                break;
        }

        SDL_RenderClear(renderer);

        uint32_t now = SDL_GetTicks();
        double diff = (now - start) / 1000.0;
        if (diff > 0.016) {
            start = now;

            uint32_t dt = now;
            //printf("Time: %d\n", dt);
            update(buffer, RENDER_X, RENDER_Y, dt);

            SDL_UpdateTexture(texture, NULL, (uint8_t *)buffer, RENDER_X * 4);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
