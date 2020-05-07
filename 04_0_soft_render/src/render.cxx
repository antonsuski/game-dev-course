#include "render.hxx"
namespace render
{
render::~render() {}

line_render::line_render(canvas& b, size_t w, size_t h)
    : buffer(b)
    , width(w)
    , height(h)
{
}

line_render::~line_render() {}

void line_render::clear(color& c)
{
    for (size_t counter = 0; counter < buffer.pixel_map.size(); ++counter)
    {
        buffer.pixel_map.at(counter) = c;
    }
}

void line_render::set_pixel(pixel& p)
{
    if (buffer_size < p.location.x_pos * p.location.y_pos)
    {
        throw(std::runtime_error("buffer overflow"));
    }

    size_t pos               = p.location.y_pos * width + p.location.x_pos;
    buffer.pixel_map.at(pos) = p.color;
}

void line_render::draw(pixels p_map, color& c)
{
    for (auto it : p_map)
    {
        pixel tmp_p{ { c }, { it.x_pos, it.y_pos } };
        set_pixel(tmp_p);
    }
}

pixels line_render::get_points(point& a, point& b)
{
    point  tmp_point{ a.x_pos, a.y_pos };
    pixels result;

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
    return result;
}

} // namespace render
