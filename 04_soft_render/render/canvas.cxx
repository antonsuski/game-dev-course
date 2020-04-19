#include "canvas.hxx"
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>

namespace render
{
void canvas::load_pixel_map(const std::string& path)
{
    std::ifstream in(path, std::ios::binary);
    std::string   format_c;
    uint8_t       wigth_c, height_c, color_deep_c;
    in >> format_c >> wigth_c >> height_c >> color_deep_c >> std::ws;
    in.read(reinterpret_cast<char*>(&pixel_map), buffer_size);
}

void canvas::save_pixel_map(const std::string& path)
{
    std::ofstream file_out(path, std::ios::binary);
    file_out << "P6\n" << width << ' ' << height << '\n' << color_deep << '\n';
    file_out.write(reinterpret_cast<char*>(&pixel_map),
                   sizeof(color) * buffer_size);
}

void canvas::clear_pixel_map()
{
    for (size_t i = 0; i < buffer_size; ++i)
    {
        pixel_map.at(i) = { 0, 0, 0 };
    }
}

void soft_render::set_pixel(pixel& p, std::array<color, buffer_size>& pixel_map)
{
    if (buffer_size < p.location.x_pos * p.location.y_pos)
    {
        throw(std::runtime_error("buffer overflow"));
    }

    size_t pos        = p.location.y_pos * width + p.location.x_pos;
    pixel_map.at(pos) = p.color;
}

void soft_render::clear(color& p, std::array<color, buffer_size>& pixel_map)
{
    for (size_t counter = 0; counter < pixel_map.size(); ++counter)
    {
        pixel_map.at(counter) = p;
    }
}

void soft_render::get_line_pixels(point& a, point& b,
                                  std::vector<point>& result)
{
    point tmp_point{ a.x_pos, a.y_pos };

    int x0 = static_cast<int>(a.x_pos);
    int y0 = static_cast<int>(a.y_pos);

    int x1 = static_cast<int>(b.x_pos);
    int y1 = static_cast<int>(b.y_pos);

    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int delta_x_abs = std::abs(x1 - x0);
    int delta_y_abs = std::abs(y1 - y0);

    int accertion = 0;

    if (delta_x_abs >= delta_y_abs)
    {
        int y         = y0;
        int direction = delta_y != 0 ? (delta_y > 0 ? 1 : -1) : 0;
        for (int x = x0; delta_x > 0 ? x <= x1 : x >= x1;
             delta_x > 0 ? x++ : x--)
        {
            tmp_point.x_pos = static_cast<size_t>(x);
            tmp_point.y_pos = static_cast<size_t>(y);
            result.push_back(tmp_point);

            accertion += delta_y_abs;
            if (accertion >= delta_x_abs)
            {
                accertion -= delta_x_abs;
                y += direction;
            }
        }
    }
    else
    {
        int x         = x0;
        int direction = delta_x != 0 ? (delta_x > 0 ? 1 : -1) : 0;
        for (int y = y0; delta_y > 0 ? y <= y1 : y >= y1;
             delta_y > 0 ? y++ : y--)
        {
            tmp_point.x_pos = static_cast<size_t>(x);
            tmp_point.y_pos = static_cast<size_t>(y);
            result.push_back(tmp_point);

            accertion += delta_x_abs;
            if (accertion >= delta_y_abs)
            {
                accertion -= delta_y_abs;
                x += direction;
            }
        }
    }
}

void soft_render::set_line(std::vector<point>& v, color& c,
                           std::array<color, buffer_size>& pixel_map)
{
    for (auto it : v)
    {
        render::pixel tmp{ { c }, { it.x_pos, it.y_pos } };
        set_pixel(tmp, pixel_map);
    }
}

void soft_render::draw_triangle(point& a, point& b, point& c, color& clr,
                                std::array<color, buffer_size>& buffer)
{

    std::vector<point> line_1, line_2, line_3;
    get_line_pixels(a, b, line_1);
    get_line_pixels(b, c, line_2);
    get_line_pixels(c, a, line_2);

    set_line(line_1, clr, buffer);
    set_line(line_2, clr, buffer);
    set_line(line_3, clr, buffer);
}

class rasterezator
{
};

} // namespace render
