#pragma once

#include <iostream>

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
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

    uniform()
        : u0(0.f)
        , u1(0.f)
        , u2(0.f)
        , u3(0.f)
    {
    }
    uniform(float a, float b, float c, float d)
        : u0(a)
        , u1(b)
        , u2(c)
        , u3(d)
    {
    }
};

struct ENGINE_DECLSPEC m_2x3
{
    m_2x3();
    static m_2x3 identiry();
    static m_2x3 scale(float scale);
    static m_2x3 scale(float sx, float sy);
    static m_2x3 rotation(float thetha);
    static m_2x3 move(const v_2& delta);
    v_2          col0;
    v_2          col1;
    v_2          delta;
};

struct ENGINE_DECLSPEC m_3x3
{
    m_3x3();
    static m_3x3 identiry();
    static m_3x3 scale(float scale);
    static m_3x3 scale(float sx, float sy);
    static m_3x3 rotation(float thetha);
    static m_3x3 move(const v_3& delta);
    v_3          col0;
    v_3          col1;
    v_3          delta;
};

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_2&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_2&);

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_3&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_3&);

ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, v_8&);
ENGINE_DECLSPEC std::istream& operator>>(std::istream& is, triangle&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const v_8&);
ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const triangle&);

ENGINE_DECLSPEC v_2 operator+(const v_2& left, const v_2 right);
ENGINE_DECLSPEC v_3 operator+(const v_3& left, const v_3 right);
ENGINE_DECLSPEC v_8 operator+(const v_8& left, const v_8 right);

} // namespace engine
