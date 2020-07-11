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

    explicit shader(const char*, const char*);

    void use();
    void set_uniform();
};
} // namespace engine
