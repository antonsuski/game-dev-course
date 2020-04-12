#include "canvas.hxx"
#include <fstream>
#include <iostream>
#include <string>

namespace render
{
std::fstream& operator<<(std::fstream& file, const pixel& p)
{
    file << p.color.red << " " << p.color.green << " " << p.color.blue << " ";
    if (p.location.x_pos > 319)
    {
        file << std::endl;
    }
    return file;
}
std::fstream& operator>>(std::fstream& file, pixel& p)
{
    file.write((char*)&p, sizeof(pixel));
}

void canvas::load_pixel_map(const std::string) {}
} // namespace render
