#if !defined(TEXTURE_H)
#define TEXTURE_H

#include <cstdint>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

class Texture {
public:
    uint32_t width, height;

    Texture(uint32_t width, uint32_t height);
    ~Texture();
    void set_pixel(uint32_t x, uint32_t y, Color c);
    Color get_pixel(uint32_t x, uint32_t y);
    const uint8_t *const data();
private:
    bool isInit;
    uint8_t *data_buffer;
};

#endif // TEXTURE_H
