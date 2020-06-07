#pragma once

#include <array>
#include <iostream>
#include <string_view>

#include "shader.hxx"

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

struct ENGINE_DECLSPEC v_2
{
    float x;
    float y;

    v_2()
        : x(0.f)
        , y(0.f)
    {
    }

    v_2(float x_, float y_)
        : x(x_)
        , y(y_)
    {
    }
};

struct ENGINE_DECLSPEC v_3
{
    float x;
    float y;
    float z;

    v_3()
        : x(0.f)
        , y(0.f)
        , z(0.f)
    {
    }

    v_3(float x_, float y_, float z_)
        : x(x_)
        , y(y_)
        , z(z_)
    {
    }
};

struct ENGINE_DECLSPEC v_8
{
    v_8()
        : x(0.f)
        , y(0.f)
        , z(0.f)
        , r(0.f)
        , g(0.f)
        , b(0.f)
        , s(0.f)
        , t(0.f)
    {
    }

    v_8(float x_, float y_, float z_, float r_, float g_, float b_, float s_,
        float t_)
        : x(x_)
        , y(y_)
        , z(z_)
        , r(r_)
        , g(g_)
        , b(b_)
        , s(s_)
        , t(t_)

    {
    }

    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float s;
    float t;
};

struct ENGINE_DECLSPEC triangle
{
    triangle()
    {
        v[0] = v_8();
        v[1] = v_8();
        v[2] = v_8();
    }
    v_8 v[3];
};

struct ENGINE_DECLSPEC uniform
{
public:
    float u0;
    float u1;
    float u2;
    float u3;
};

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_8&);
ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, triangle&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_8&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const triangle&);

class ENGINE_DECLSPEC core
{
public:
    virtual ~core()                                            = 0;
    virtual bool  init()                                       = 0;
    virtual bool  read_event(event&)                           = 0;
    virtual void  render()                                     = 0;
    virtual void  swap_buffers()                               = 0;
    virtual void  uninitialize()                               = 0;
    virtual void  render_my_triangle(const triangle&, shader&) = 0;
    virtual void  render_grid(shader&)                         = 0;
    virtual void  sempling(const triangle& text_coord)         = 0;
    virtual float get_time_for_init()                          = 0;
    virtual bool  init_my_opengl()                             = 0;
    virtual bool  load_texture(std::string_view)               = 0;
};
ENGINE_DECLSPEC engine::core* create_engine();
ENGINE_DECLSPEC void          destroy_engine(engine::core* e);
} // namespace engine
