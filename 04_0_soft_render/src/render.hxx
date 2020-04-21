#pragma once

#include "canvas.hxx"
#include <vector>

#ifndef RENDER_DECLSPEC
#define RENDER_DECLSPEC
#endif

namespace render
{
using pixels = std::vector<point>;

class RENDER_DECLSPEC render
{
public:
    virtual ~render();
    virtual void clear(color&)        = 0;
    virtual void set_pixel(pixel&)    = 0;
    virtual void draw(pixels, color&) = 0;

    virtual pixels get_points(point&, point&) = 0;
};

class line_render : public render
{
public:
    line_render(canvas&, size_t, size_t);
    ~line_render() override;

    void clear(color&) override;
    void set_pixel(pixel&) override;
    void draw(pixels, color&) override;

    pixels get_points(point&, point&) override;

private:
    canvas&      buffer;
    const size_t width;
    const size_t height;
};

} // namespace render
