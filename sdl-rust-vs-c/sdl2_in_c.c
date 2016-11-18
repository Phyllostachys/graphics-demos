#include <stdio.h>
#include "SDL.h"

int main(int argc, char** argv)
{
    int res;
    SDL_Window* window;
    SDL_Renderer* renderer;

    res = SDL_Init(SDL_INIT_EVERYTHING);
    if (res != 0) {
        printf("Could not init SDL");
        return -1;
    }

    printf("Creating Window...\n");
    char* title = "C SDL2 Test";
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_OPENGL);
    if (window == 0) {
        printf("Could not create window");
        return -1;
    }

    printf("Creating Renderer...\n");
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == 0) {
        printf("Could not create renderer");
        return -1;
    }

    printf("Presenting...\n");
    SDL_RenderPresent(renderer);

    printf("Delaying...\n");
    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
