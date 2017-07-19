#include "texture.h"

#include <cstdint>

Texture::Texture(uint32_t width, uint32_t height) {
    this->width = width;
    this->height = height;
    data_buffer = new uint8_t[width * height * 4];
    if (data_buffer) isInit = true;
}

Texture::~Texture() {
    delete [] data_buffer;
}

void Texture::set_pixel(uint32_t x, uint32_t y, Color c) {
    if (isInit) {
        uint32_t offset = (y*(width*4)) + (x*4);
        data_buffer[offset + 0] = c.red;
        data_buffer[offset + 1] = c.green;
        data_buffer[offset + 2] = c.blue;
        data_buffer[offset + 3] = c.alpha;
    }
}

Color Texture::get_pixel(uint32_t x, uint32_t y) {
    Color c;
    if (isInit) {
        uint32_t offset = (y*(width*4)) + (x*4);
        c.red   = data_buffer[offset + 0];
        c.green = data_buffer[offset + 1];
        c.blue  = data_buffer[offset + 2];
        c.alpha = data_buffer[offset + 3];
    } else {
        c.red = 0;
        c.green = 0;
        c.blue = 0;
        c.alpha = 255;
    }
    return c;
}

const uint8_t *const Texture::data() {
    return (const uint8_t *const)data_buffer;
}
