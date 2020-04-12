#pragma once

#include <array>
#include <fstream>
#include <string>

#ifndef RENDER_DECLSPEC
#define RENDER_DECLSPEC
#endif

namespace render
{
constexpr static int    weight      = 320;
constexpr static int    height      = 240;
constexpr static size_t buffer_size = weight * height;

struct RENDER_DECLSPEC point
{
    int x_pos;
    int y_pos;

    point(const int& x, const int& y)
        : x_pos(x)
        , y_pos(y)
    {
    }
    point(const render::point& p)
        : x_pos(p.x_pos)
        , y_pos(p.y_pos)
    {
    }
};

struct RENDER_DECLSPEC color
{
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    color(const uint8_t& r, const uint8_t& g, const uint8_t& b)
        : red(r)
        , green(g)
        , blue(b)
    {
    }
    color(const color& c)
        : red(c.red)
        , green(c.green)
        , blue(c.blue)
    {
    }
};

struct RENDER_DECLSPEC pixel
{
    render::color color;
    render::point location;

    pixel(const render::color& c, const render::point& l)
        : color(c.red, c.green, c.blue)
        , location(l.x_pos, l.y_pos)
    {
    }
};

RENDER_DECLSPEC std::fstream& operator<<(std::fstream& file, const pixel& p);
RENDER_DECLSPEC std::fstream& operator>>(std::fstream& file, pixel& p);

class RENDER_DECLSPEC canvas
{
public:
    std::array<pixel, buffer_size> pixel_map;

    void load_pixel_map(const std::string);
    void save_pixel_map();
};

} // namespace render
