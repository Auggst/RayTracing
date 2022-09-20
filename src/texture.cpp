#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image_texture::Image_texture(const char *filename)
{
    auto components_per_pixel = bytes_per_pixel;

    _data = stbi_load(filename, &_width, &_height, &components_per_pixel, components_per_pixel);
    if (!_data)
    {
        std::cerr << "ERROR: Could not load texture image file " << filename << ".\n";
        _width = _height = 0;
    }
    _bytes_per_scanline = bytes_per_pixel * _width;
    std::cout << "Image Loading Done!" << std::endl;
}