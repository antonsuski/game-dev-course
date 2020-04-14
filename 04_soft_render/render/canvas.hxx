#pragma once

#include <array>
#include <fstream>
#include <string_view>

#ifndef RENDER_DECLSPEC
#define RENDER_DECLSPEC
#endif

namespace render
{
constexpr static std::string_view format      = "P6 ";
constexpr static size_t           width       = 255;
constexpr static size_t           height      = 255;
constexpr static size_t           color_deep  = 255;
constexpr static size_t           buffer_size = width * height;

struct RENDER_DECLSPEC point
{
    size_t x_pos;
    size_t y_pos;
};

struct RENDER_DECLSPEC color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct RENDER_DECLSPEC pixel
{
    render::color color;
    render::point location;
};

class RENDER_DECLSPEC canvas
{
public:
    std::array<color, buffer_size> pixel_map;

    void load_pixel_map(const std::string);
    void save_pixel_map(const std::string);
    void clear_pixel_map();
    void set_pixel(const pixel&);
};

} // namespace render
