#pragma once

#include <array>
#include <iostream>
#include <string_view>

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class event
{
public:
    enum key
    {
        up,
        down,
        left,
        right,
        select,
        start,
        button_1,
        button_2,
        mouse_move,
        turn_off
    } key;
    std::string_view name;
    bool             is_running;

    event(enum key event_key, std::string_view event_name,
          bool is_event_running)
        : key(event_key)
        , name(event_name)
        , is_running(is_event_running)
    {
    }
    event() {}
};

ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const event& e);

struct ENGINE_DECLSPEC vertex
{
    vertex()
        : x(0.f)
        , y(0.f)
        , z(0.f)
        , r(0.f)
        , g(0.f)
        , b(0.f)
        , s(0.f)
        , k(0.f)
    {
    }

    vertex(float x_, float y_, float z_, float r_, float g_, float b_, float s_,
           float k_)
        : x(x_)
        , y(y_)
        , z(z_)
        , r(r_)
        , g(g_)
        , b(b_)
        , s(s_)
        , k(k_)

    {
    }

    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float s;
    float k;
};

struct ENGINE_DECLSPEC triangle
{
    triangle()
    {
        v[0] = vertex();
        v[1] = vertex();
        v[2] = vertex();
    }
    vertex v[3];
};

struct ENGINE_DECLSPEC uniform
{
public:
    float u0;
    float u1;
    float u2;
    float u3;
};

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, vertex&);
ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, triangle&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const vertex&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const triangle&);

class ENGINE_DECLSPEC core
{
public:
    virtual ~core()                                    = 0;
    virtual bool  init()                               = 0;
    virtual bool  read_event(event&)                   = 0;
    virtual void  render()                             = 0;
    virtual void  swap_buffers()                       = 0;
    virtual void  uninitialize()                       = 0;
    virtual void  render_triangle(const triangle&)     = 0;
    virtual void  render_my_triangle(const triangle&)  = 0;
    virtual void  render_grid()                        = 0;
    virtual void  sempling(const triangle& text_coord) = 0;
    virtual float get_time_for_init()                  = 0;
    virtual bool  init_my_opengl()                     = 0;
    virtual bool  load_texture(std::string_view)       = 0;
};
ENGINE_DECLSPEC engine::core* create_engine();
ENGINE_DECLSPEC void          destroy_engine(engine::core* e);
} // namespace engine
