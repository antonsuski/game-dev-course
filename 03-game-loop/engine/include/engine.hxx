#pragma once

#include <array>
#include <iostream>
#include <string_view>

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{

enum class event
{
    shut_down,
    up_pushed,
    down_pushed,
    left_pushed,
    right_pushed,
    start_pushed,
    select_pushed,
    button_1_pushed,
    button_2_pushed,
    up_released,
    down_released,
    left_released,
    right_released,
    start_released,
    select_released,
    button_1_released,
    button_2_released
};

ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const event e);

class ENGINE_DECLSPEC core
{
public:
    virtual ~core()                   = 0;
    virtual bool init()               = 0;
    virtual bool handle_event(event&) = 0;
};
ENGINE_DECLSPEC engine::core* create_engine();
ENGINE_DECLSPEC void          destroy_engine(engine::core* e);
} // namespace engine
