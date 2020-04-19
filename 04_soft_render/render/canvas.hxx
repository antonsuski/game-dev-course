#pragma once

#include <array>
#include <fstream>
#include <map>
#include <string_view>
#include <vector>

#ifndef RENDER_DECLSPEC
#define RENDER_DECLSPEC
#endif

namespace render
{
constexpr static std::string_view format      = "P6 ";
constexpr static size_t           width       = 256;
constexpr static size_t           height      = 256;
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

    void load_pixel_map(const std::string&);
    void save_pixel_map(const std::string&);
    void clear_pixel_map();
};

class RENDER_DECLSPEC soft_render
{

public:
    std::vector<point>                          vertex_buffer;
    std::map<int, std::vector<point>::iterator> index_buffer;

    void add_vertex_to_VB(point&);

    static void set_pixel(pixel& p, std::array<color, buffer_size>& pixel_map);
    static void clear(color& p, std::array<color, buffer_size>& pixel_map);
    static void set_line(std::vector<point>& v, color& c,
                         std::array<color, buffer_size>& pixel_map);
    static void get_line_pixels(point& a, point& b, std::vector<point>& result);
    static void draw_triangle(point&, point&, point&, color& clr,
                              std::array<color, buffer_size>& buffer);
};

} // namespace render
