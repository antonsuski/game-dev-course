#include "canvas.hxx"
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>

namespace render
{
void canvas::load_pixel_map(const std::string path)
{
    std::ifstream in(path, std::ios::binary);
    std::string   format_c;
    uint8_t       wigth_c, height_c, color_deep_c;
    in >> format_c >> wigth_c >> height_c >> color_deep_c >> std::ws;
    in.read(reinterpret_cast<char*>(&pixel_map), buffer_size);
}

void canvas::save_pixel_map(const std::string path)
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

void canvas::set_pixel(const pixel& p)
{
    if (buffer_size < p.location.x_pos * p.location.y_pos)
    {
        throw(std::runtime_error("buffer overflow"));
    }
    pixel_map.at(p.location.x_pos * p.location.y_pos) = p.color;
}
} // namespace render
