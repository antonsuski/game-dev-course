#include <fstream>
#include <iostream>

#include "canvas.hxx"

int main()
{
    render::canvas lol;
    lol.load_pixel_map("lol.ppm");
    lol.clear_pixel_map();
    lol.set_pixel(render::pixel{ { 255, 255, 255 }, { 150, 150 } });
    lol.save_pixel_map("lol1.ppm");
    return EXIT_SUCCESS;
}
