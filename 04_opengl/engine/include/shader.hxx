#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include <glad/glad.h>

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class ENGINE_DECLSPEC shader
{
public:
    unsigned int id;

    shader(const char*, const char*);

    void use();

    void set_bool(const std::string_view&, bool) const;
    void set_int(const std::string_view&, int) const;
    void set_float(const std::string_view&, float) const;
};
} // namespace engine
