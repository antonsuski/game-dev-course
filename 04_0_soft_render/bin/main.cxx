#include <iostream>
#include <vector>

#include "canvas.hxx"
#include "render.hxx"

int main()
{
    render::canvas             lol;
    render::soft_render        kek;
    render::color              clr{ 0, 0, 0 };
    render::color              line_c{ 0, 255, 0 };
    render::point              a{ 128, 50 };
    render::point              b{ 255, 255 };
    render::point              c{ 150, 170 };
    std::vector<render::point> vect;
    render::pixel              p{ { 255, 255, 255 }, { a.x_pos, a.y_pos } };

    render::line_render line(lol, render::width, render::height);

    lol.load_pixel_map("lol.ppm");

    kek.clear(clr, lol.pixel_map);
    kek.get_line_pixels(a, b, vect);
    kek.draw_triangle(a, b, c, line_c, lol.pixel_map);
    line.set_pixel(p);
    lol.save_pixel_map("lol1.ppm");
    return EXIT_SUCCESS;
}
